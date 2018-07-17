# Injen

Injen is a tool used within the build process to generate source from template files, using the C++ inja library and it's syntax.  The name is an amalgamation of the words "inja" and "gen" (for generator)

## Usage

The tool has a straight forward command-line interface in which you specify a number source templates to render and a json file containing the data object to use within the transformations.

`injen <source-files> [-d | --data] <json-file>`