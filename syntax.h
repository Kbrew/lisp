typedef struct SourceLocation{
	char *file_name;
	unsigned int line;
	unsigned int col;
} SourceLocation;

typedef struct Scope {
	unsigned int id;
	char *name;
	SourceLocation loc;
} Scope;

typedef struct ScopeSet{
	size_t size;
	Scope scopes[];
} ScopeSet;

typedef struct Syntax {
	Object *value;
	SourceLocation loc;
	ScopeSet *scope_set;
} Syntax;