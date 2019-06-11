```
eof
block_begin				INDENT
block_end				DEDENT
keyword					// see keyword list.
identifier				[A-Za-z_][A-Za-z0-9_]*
integer					0 | [1-9][0-9]+
decimal
string
character
line_comment
block_comment_begin
block_comment_end
```


## brackets
```
lparen					(
rparen					)
lsquare_bracket			[
rsquare_bracket			]
langle_bracket			<
rangle_bracket			>
```

## operators
```
thin_arrow           ->
thick_arrow				=>
dot						.
comma					   ,
colon					   :
scolon					;
placeholder				$
extension				@
```

## logical operators
```
logical_and 			&&
logical_not 			!
logical_or 				||
eq						==
neq						!=
lt						<
gt						>
lteq					<=
gteq					>=
```

## arithmetic operators
```
inc						++
dec						--
assignment				=
plus					+
minus					-
multiply				*
divide					/
modulo					%
compound_plus			+=
compound_minus			-=
compound_multiply		*=
compound_divide			/=
compound_modulo			%=
```

## Correspondance

"="
 - "=="
 - "=>"
 - "="

"+"
 - "++"
 - "+="
 - "+"
