# Systems-Programming

For the purposes of this assignment, you will write a mini-database that uses basic file I/O.  You may choose any file format you like, including flat file, XML, JSON, comma separated values, binary files, or a format of your own choosing, but you must support the following:
Some SQL-like operations, namely:
SELECT * FROM TableName WHERE Field1="value"
DELETE FROM TableName WHERE Field1="value"
UPDATE TableName SET Field1="new value" WHERE Field2="value"
SELECT * FROM TableName JOIN TableName2 ON TableName.Field1 = TableName.Field2
INSERT INTO TableName (Field1="value", Field2="value", ...)
Creation and deletion of multiple tables per database:
CREATE TABLE TableName FIELDS [Field1, Field2, ...]
DROP TABLE TableName
In addition, your program must be able to select a database, creating one if it does not exist, via a command-line parameter.
