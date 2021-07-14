# DemoCodeV2

New Matrix- and Vector-operation classes, 
for seamless mathematical operations.

Matrix-class now inherits from Vector-class, for
continuous (!) memory-allocation. Performance! 
('Zusammenhaengender, benachbarter Speicher')

Mathematical operators overloaded, in member- and non-member operators.
Access to Vector by [index].
Access to Matrix by (row, col).

Vector-class in charge of allocating memory. 

Matrix-class handles indexing-conversion of (row,col) into 
single-index (array). Maintains function-pointer to indexing-function.
Pointer gets re-assigned only when matrix is transposed.
