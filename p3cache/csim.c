#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cachelab.h"

typedef unsigned long long int address;

typedef struct {
  int sBits;
  int sets;
  int lines;
  int bBits;
  int bSize;  
} cache_info;

typedef struct {
  int vbit;
  address tag;
  int timestamp;
} line;

typedef struct {
  line *setLines;
} set;

typedef struct {
  set *sets;
} Cache;

int v, hits, misses, evictions;


void printUsage(char *path) {
  printf( "Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", path);
  printf( "Options:\n" );
  printf( "  -h         Print this help message.\n" );
  printf( "  -v         Optional verbose flag.\n" );
  printf( "  -s <num>   Number of set index bits.\n" );
  printf( "  -E <num>   Number of lines per set.\n" );
  printf( "  -b <num>   Number of block offset bits.\n" );
  printf( "  -t <file>  Trace file.\n" );
  printf( "\nExamples:\n" );
  printf( "  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", path);
  printf( "  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", path);
}

Cache* makeCache(cache_info c) {
  Cache *cache;
  cache = (Cache*)malloc(sizeof(Cache));
  cache->sets = (set*)malloc(sizeof(set)*c.sets);
  for (int i = 0; i<c.sets; i++) {
    cache->sets[i].setLines = (line*)malloc(sizeof(line)*c.lines);
    for (int j = 0; j<c.lines; j++) {
      cache->sets[i].setLines[j].vbit = 0;
      cache->sets[i].setLines[j].tag = 0;
      cache->sets[i].setLines[j].timestamp = 0;
    }
  }
  return cache;
}

void freeCache(Cache *cache, cache_info c) {
  for (int i = 0; i<c.sets; i++) {
    free(cache->sets[i].setLines);
  }
  free(cache->sets);
  free(cache);
}

void simulate(cache_info c, char *file) {
  Cache *cache = makeCache(c);
  FILE *trace = fopen(file, "r");
  char instr;
  address addr;
  int size;
  long long int timestamp, oldest;
  timestamp = 0;
  int hit, miss, evic, emptyLine;
  while (fscanf(trace, " %c %llx,%d", &instr, &addr, &size) == 3) {
    if (instr == 'I') {
      continue;
    }
    hit = evic = 0;
    miss = 1;
    emptyLine = -1;
    int setIndex = (addr>>c.bBits)&(c.sets-1);
    address tag = addr>>(c.bBits+c.sBits);
    set currSet = cache->sets[setIndex];
    int toEvict = 0;
    oldest = currSet.setLines[0].timestamp;
    for (int l = 0; l<c.lines; l++) {
      if (currSet.setLines[l].vbit == 1) {
	if (currSet.setLines[l].tag == tag) {
	  hit = 1;
	  miss = 0;
	  currSet.setLines[l].timestamp = timestamp;
	  timestamp++;
	} else if (currSet.setLines[l].timestamp < oldest) {
	    oldest = currSet.setLines[l].timestamp;
	    toEvict = l;
	}
      } else if (emptyLine == -1) {
	emptyLine = l;
      }
    }
    if (miss) {
      if (emptyLine != -1) {
	currSet.setLines[emptyLine].vbit = 1;
	currSet.setLines[emptyLine].tag = tag;
	currSet.setLines[emptyLine].timestamp = timestamp;
	timestamp++;
      } else if (emptyLine == -1) {
	evic = 1;
	currSet.setLines[toEvict].tag = tag;
	currSet.setLines[toEvict].timestamp = timestamp;
	timestamp++;
      }
    }
    
    if (v) {
      printf("%c %llx,%d ", instr, addr, size);
    }
    
    switch(instr) {
      case 'M':
	if(hit) {
	  hits +=2;
	  if (v) {
	    printf("hit hit\n");
	  }
	} else if (evic) {
	  misses++;
	  evictions++;
	  hits++;
	  if (v) {
	    printf("miss eviction hit\n");
	  }
	} else {
	  misses++;
	  hits++;
	  if (v) {
	    printf("miss hit\n");
	  }
	}
	break;
      default:
	if (hit) {
	  hits++;
	  if (v) {
	    printf("hit\n");
	  }
	} else if (evic) {
	  misses++;
	  evictions++;
	  if (v) {
	    printf("miss eviction\n");
	  }
	} else {
	  misses++;
	  if (v) {
	    printf("miss\n");
	  }
	}
    }
  }
  fclose(trace);
  freeCache(cache, c);
}

int main(int argc, char **argv) {
  hits = misses = evictions = v = 0;
  char *file;
  cache_info cache = {0, 0, 0, 0, 0};
  if (argc<2) {
    fprintf(stderr, "%s: missing required command line arguments\n", argv[0]);
    printUsage(argv[0]);
    exit(1);
  }
  int i = 1;
  while (i < argc && argv[i+1]) {
    if (strcmp("-h", argv[i]) == 0) {
      printUsage(argv[0]);
      exit(1);
    } else if (strcmp("-v", argv[i]) == 0) {
      v = 1;
      i--;
    } else if (strcmp("-s", argv[i]) == 0) {
      cache.sBits = atoi(argv[i+1]);
    } else if (strcmp("-E", argv[i]) == 0) {
      cache.lines = atoi(argv[i+1]);
    } else if (strcmp("-b", argv[i]) == 0) {
      cache.bBits = atoi(argv[i+1]);
    } else if (strcmp("-t", argv[i]) == 0) {
      file = argv[i+1];
    } else {
      fprintf(stderr, "%s: invalid option -- '%s'\n", argv[0], argv[i]);
      printUsage(argv[0]);
      exit(1);
    }
    i += 2;
  }
  if (cache.sBits < 1 || cache.lines < 1 || cache.bBits < 1 || file == NULL) {
    fprintf(stderr, "%s: missing required command line argument\n", argv[0]);
    printUsage(argv[0]);
    exit(1);
  }
  cache.sets = 1<<cache.sBits;
  cache.bSize = 1<<cache.bBits;
  
  simulate(cache, file); 
  printSummary(hits, misses, evictions);
  return 0;
}
