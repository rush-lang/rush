
- Verbatim strings &mdash; beginning with an ampersat (`@`) &mdash; may specify an identifier between it and the opening quotation mark.  This is for tools and IDE's to enrich the development experience.

```rush
let query = @sql"select * from users"
```

## Multiline Strings

- Multiline strings may use the indicators that yaml uses to control new lines and indentation.

```rush
with db = new sql.connection(connection_string)
   dyn users = await db.query(@sql"""
      select * from users
      where active=1
      order by created_at
   """)

   for {id, name} in users
      print("$id: $name")

let jj = json.parse(@json""" >-
{
   "id": 1,
   "role": "admin",
   "claims": {
      { "email": "reegan.layzell@rush-lang.org" },
      { "username": "reegan.layzell" },
   }
}""")
```
