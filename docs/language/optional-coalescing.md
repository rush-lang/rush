# Nil-coalescing

```rush
return result ?! mapper.map(result) ## returns the left hand side if nil
return result ?? new Something() 	## returns the right hand side if nil

array?.[0]  	## indexing optional values
foo?.() 		   ## calling optional functions
value?.member	## member access
```
