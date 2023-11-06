# Coding Conventions and Guidelines

The following article presents the guidelines that developers writing code in Rush are encouraged to follow. Here we list the naming and layout conventions that when followed, provide a consistent coding experience with Rush.

## Naming Convention

Rush is a case-sensitive language, however, keywords and nearly all identifiers use the `snake_case` naming convention, except for generic type identifiers which use `PascalCase`.

The rationale behind this is ease of typing, word clarity, the aesthetic, and reliance on editing tools to distinguish between the various types of identifiers. Rush is designed to be a maximally productive language, contorting the eyes and fingers to type in odd patterns seemed to fly in the face of that.

### Naming Conflicts

Occasionally, naming conflicts arise such as naming a variable the same as it's type, or naming something that is already a reserved keyword. In which case, we suffix the name with an underscore.

~~It is well known that creating identifiers with clear and concise names is good practice, and sometimes the best name for a variable is that of its type. While you could name a variable the same as its type, that variable would then hide the name of the type for the remainder of the scope. For this reason the recommended approach is to suffix the variable name with an underscore.~~

```rush
let window_ = new window()
```

### Private Members

```rush
class foo
   private let _bar: bar
```

## Layout Conventions

################## FROM C# DOCS

Use the default Code Editor settings (smart indenting, four-character indents, tabs saved as spaces). For more information, see Options, Text Editor, C#, Formatting.

Write only one statement per line.

Write only one declaration per line.

If continuation lines are not indented automatically, indent them one tab stop (four spaces).

Add at least one blank line between method definitions and property definitions.

Use parentheses to make clauses in an expression apparent, as shown in the following code.
