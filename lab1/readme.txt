Jose Reyes

Exercise 2
svn log output:
------------------------------------------------------------------------
r2 | mwachs | 2017-09-27 23:04:35 -0500 (Wed, 27 Sep 2017) | 1 line

creating lab1
------------------------------------------------------------------------

Exercise 3
svn diff output:
Index: names.txt
===================================================================
--- names.txt	(revision 6)
+++ names.txt	(working copy)
@@ -1 +1 @@
-Reyes
+Jose Reyes

Revision 6 is associated with the just family name file
Revision 7 is associated with the full name revision of the file

svn diff -r 6:7 output:
Index: names.txt
===================================================================
--- names.txt	(revision 6)
+++ names.txt	(revision 7)
@@ -1 +1 @@
-Reyes
+Jose Reyes

The "working copy" description is changed to the revision number being compared.

Exercise 4
svn merge -r 7:6 names.txt output
Index: names.txt
===================================================================
--- names.txt	(revision 7)
+++ names.txt	(working copy)
@@ -1 +1 @@
-Jose Reyes
+Reyes

