/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of
the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
#ifndef __SCRIPT_COMPILER_H__
#define __SCRIPT_COMPILER_H__

const char* const RESULT_STRING = "<RESULT>";

typedef struct opcode_s {
	// RB begin
	const char* name;
	const char* opname;
	// RB end
	int			priority;
	bool		rightAssociative;
	idVarDef*	type_a;
	idVarDef*	type_b;
	idVarDef*	type_c;
} opcode_t;

// These opcodes are no longer necessary:
// OP_PUSH_OBJ:
// OP_PUSH_OBJENT:

enum {
	OP_RETURN,

	OP_UINC_F,
	OP_UINCP_F,
	OP_UDEC_F,
	OP_UDECP_F,
	OP_COMP_F,

	OP_MUL_F,
	OP_MUL_V,
	OP_MUL_FV,
	OP_MUL_VF,
	OP_DIV_F,
	OP_MOD_F,
	OP_ADD_F,
	OP_ADD_V,
	OP_ADD_S,
	OP_ADD_FS,
	OP_ADD_SF,
	OP_ADD_VS,
	OP_ADD_SV,
	OP_SUB_F,
	OP_SUB_V,

	OP_EQ_F,
	OP_EQ_V,
	OP_EQ_S,
	OP_EQ_E,
	OP_EQ_EO,
	OP_EQ_OE,
	OP_EQ_OO,

	OP_NE_F,
	OP_NE_V,
	OP_NE_S,
	OP_NE_E,
	OP_NE_EO,
	OP_NE_OE,
	OP_NE_OO,

	OP_LE,
	OP_GE,
	OP_LT,
	OP_GT,

	OP_INDIRECT_F,
	OP_INDIRECT_V,
	OP_INDIRECT_S,
	OP_INDIRECT_ENT,
	OP_INDIRECT_BOOL,
	OP_INDIRECT_OBJ,

	OP_ADDRESS,

	OP_EVENTCALL,
	OP_OBJECTCALL,
	OP_SYSCALL,

	OP_STORE_F,
	OP_STORE_V,
	OP_STORE_S,
	OP_STORE_ENT,
	OP_STORE_BOOL,
	OP_STORE_OBJENT,
	OP_STORE_OBJ,
	OP_STORE_ENTOBJ,

	OP_STORE_FTOS,
	OP_STORE_BTOS,
	OP_STORE_VTOS,
	OP_STORE_FTOBOOL,
	OP_STORE_BOOLTOF,

	OP_STOREP_F,
	OP_STOREP_V,
	OP_STOREP_S,
	OP_STOREP_ENT,
	OP_STOREP_FLD,
	OP_STOREP_BOOL,
	OP_STOREP_OBJ,
	OP_STOREP_OBJENT,

	OP_STOREP_FTOS,
	OP_STOREP_BTOS,
	OP_STOREP_VTOS,
	OP_STOREP_FTOBOOL,
	OP_STOREP_BOOLTOF,

	OP_UMUL_F,
	OP_UMUL_V,
	OP_UDIV_F,
	OP_UDIV_V,
	OP_UMOD_F,
	OP_UADD_F,
	OP_UADD_V,
	OP_USUB_F,
	OP_USUB_V,
	OP_UAND_F,
	OP_UOR_F,

	OP_NOT_BOOL,
	OP_NOT_F,
	OP_NOT_V,
	OP_NOT_S,
	OP_NOT_ENT,

	OP_NEG_F,
	OP_NEG_V,

	OP_INT_F,
	OP_IF,
	OP_IFNOT,

	OP_CALL,
	OP_THREAD,
	OP_OBJTHREAD,

	OP_PUSH_F,
	OP_PUSH_V,
	OP_PUSH_S,
	OP_PUSH_ENT,
	OP_PUSH_OBJ,
	OP_PUSH_OBJENT,
	OP_PUSH_FTOS,
	OP_PUSH_BTOF,
	OP_PUSH_FTOB,
	OP_PUSH_VTOS,
	OP_PUSH_BTOS,

	OP_GOTO,

	OP_AND,
	OP_AND_BOOLF,
	OP_AND_FBOOL,
	OP_AND_BOOLBOOL,
	OP_OR,
	OP_OR_BOOLF,
	OP_OR_FBOOL,
	OP_OR_BOOLBOOL,

	OP_BITAND,
	OP_BITOR,

	OP_BREAK,	 // placeholder op.  not used in final code
	OP_CONTINUE, // placeholder op.  not used in final code

	NUM_OPCODES
};

/*!
	\class idCompiler
	\brief Compiler class for processing and generating bytecode from source code.

	This class implements a full compiler for processing source code and generating bytecode representations. It handles parsing of statements, expressions, function calls, and control flow constructs
   while managing symbol tables and type information. The compiler processes tokens from an input stream, performs semantic analysis, and emits bytecode operations for execution. It supports various
   language constructs including variable declarations, function definitions, object method calls, event handling, and control structures like loops and conditionals. The class manages compilation
   state including scope handling, error reporting, and code generation for different operation types including function calls, jumps, and immediate values. The compiler also handles type checking and
   validation during parsing, ensures correct parameter passing for function calls, and manages compilation of object-oriented constructs.

*/
class idCompiler
{
private:
	static bool		   punctuationValid[256];
	// RB begin
	static const char* punctuation[];
	// RB end

	idParser		   parser;
	idParser*		   parserPtr;
	idToken			   token;

	idTypeDef*		   immediateType;
	eval_t			   immediate;

	bool			   eof;
	bool			   console;
	bool			   callthread;
	int				   braceDepth;
	int				   loopDepth;
	int				   currentLineNumber;
	int				   currentFileNumber;
	int				   errorCount;

	idVarDef*		   scope;	 // the function being parsed, or NULL
	const idVarDef*	   basetype; // for accessing fields

	//! Performs division of two floating-point numbers and handles division by zero error.
	float			   Divide( float numerator, float denominator );

	//! Aborts the current file load with a formatted error message.
	void			   Error( VERIFY_FORMAT_STRING const char* error, ... ) const;

	//! Prints a formatted warning message about the current line
	void			   Warning( VERIFY_FORMAT_STRING const char* message, ... ) const;

	//! Optimizes bytecode operations on constant values by pre-computing results
	idVarDef*		   OptimizeOpcode( const opcode_t* op, idVarDef* var_a, idVarDef* var_b );

	//! Emits a primitive statement and returns the variable containing its result
	idVarDef*		   EmitOpcode( const opcode_t* op, idVarDef* var_a, idVarDef* var_b );

	//! Emits a primitive statement and returns the variable containing its value.
	idVarDef*		   EmitOpcode( int op, idVarDef* var_a, idVarDef* var_b );

	//! Emits an opcode to push a variable onto the stack.
	bool			   EmitPush( idVarDef* expression, const idTypeDef* funcArg );

	//! Advances the compiler to the next token in the input stream
	void			   NextToken();

	//! Checks if the current token matches the expected string, and reports an error if not.
	void			   ExpectToken( const char* string );

	//! Checks if the current token matches the specified string and advances to the next token if there is a match.
	bool			   CheckToken( const char* string );

	//! Checks if the current token is a valid name and parses it into the provided string.
	void			   ParseName( idStr& name );

	//! Pops out of nested braces for error recovery.
	void			   SkipOutOfFunction();

	//! Skips tokens until a semicolon is found or end of file is reached.
	void			   SkipToSemicolon();

	//! Parses a variable type from the current token and returns the corresponding type definition.
	idTypeDef*		   CheckType();

	//! Parses and returns a variable type, including function types
	idTypeDef*		   ParseType();

	//! Finds an existing immediate definition with the same value as the provided evaluation data
	idVarDef*		   FindImmediate( const idTypeDef* type, const eval_t* eval, const char* string ) const;

	//! Returns an existing immediate variable with the same value or allocates a new one.
	idVarDef*		   GetImmediate( idTypeDef* type, const eval_t* eval, const char* string );

	//! Creates a definition for an index into a virtual function table
	idVarDef*		   VirtualFunctionConstant( idVarDef* func );

	//! Creates a variable definition for a size constant.
	idVarDef*		   SizeConstant( int size );

	//! Creates a variable definition for a jump constant with the specified integer value.
	idVarDef*		   JumpConstant( int value );

	//! Creates a definition for a relative jump from one code location to another.
	idVarDef*		   JumpDef( int jumpfrom, int jumpto );

	//! Creates a variable definition for a relative jump from the current code location to the specified jump target.
	idVarDef*		   JumpTo( int jumpto );

	//! Returns a variable definition for a relative jump from the specified code location to the current code location
	idVarDef*		   JumpFrom( int jumpfrom );

	//! Parses and returns an immediate value constant from the current token.
	idVarDef*		   ParseImmediate();

	/*!
		\brief Emits bytecode for function parameters and handles result storage for function calls.

		This function processes the parameters of a function call, ensuring that the correct number and types of arguments are provided. It generates appropriate bytecode instructions for parameter
	   passing, including handling object calls and thread execution. The function also manages the allocation and storage of return values, selecting the appropriate storage operation based on the
	   return type of the function.

		\param op Operation code indicating the type of function call (e.g., OP_CALL, OP_OBJECTCALL, OP_OBJTHREAD)
		\param func Definition of the function being called
		\param startarg Starting argument index for parameter processing
		\param startsize Starting size in bytes for the parameter stack space
		\param object Object instance for object method calls (NULL for regular function calls)
		\return Definition of the result variable where the function return value will be stored
		\throws Error is thrown if the number of parameters provided is incorrect or if there is a type mismatch
	*/
	idVarDef*		   EmitFunctionParms( int op, idVarDef* func, int startarg, int startsize, idVarDef* object );

	//! Parses a function call for the given function definition and returns the parsed result.
	idVarDef*		   ParseFunctionCall( idVarDef* func );

	//! Parses an object method call expression and generates bytecode for it.
	idVarDef*		   ParseObjectCall( idVarDef* object, idVarDef* func );

	//! Parses an event call for a given object and function definition.
	idVarDef*		   ParseEventCall( idVarDef* object, idVarDef* func );

	//! Parses a system object call and generates bytecode for it
	idVarDef*		   ParseSysObjectCall( idVarDef* func );

	//! Looks up a definition by name within the compiler's scope or associated object context
	idVarDef*		   LookupDef( const char* name, const idVarDef* baseobj );

	//! Returns the def for the current token
	idVarDef*		   ParseValue();

	//! Parses and returns a term expression from the compiler's input stream.
	idVarDef*		   GetTerm();

	//! Checks if two type identifiers match exactly.
	bool			   TypeMatches( etype_t type1, etype_t type2 ) const;

	//! Retrieves a variable definition for an expression with the specified priority level from the compiler's token stream.
	idVarDef*		   GetExpression( int priority );

	//! Returns the type definition for a given event argument type.
	idTypeDef*		   GetTypeForEventArg( char argType );

	//! Patches break and continue statements within a loop starting at a given position.
	void			   PatchLoop( int start, int continuePos );

	//! Parses a return statement in the compiler, handling return values and type checking.
	void			   ParseReturnStatement();

	//! Parses a while statement in the compiler
	void			   ParseWhileStatement();

	//! Parses a for statement in the compiler.
	void			   ParseForStatement();

	//! Parses a do-while loop statement in the compiler.
	void			   ParseDoWhileStatement();

	//! Parses an if statement and generates bytecode for conditional execution.
	void			   ParseIfStatement();

	//! Parses a single statement in the compiler, handling various control flow and declaration constructs.
	void			   ParseStatement();

	//! Parses and defines a new object type with the specified name, including its fields and inheritance.
	void			   ParseObjectDef( const char* objname );

	//! Parses a function type definition and returns a type definition object.
	idTypeDef*		   ParseFunction( idTypeDef* returnType, const char* name );

	//! Parses a function definition with the specified return type and name.
	void			   ParseFunctionDef( idTypeDef* returnType, const char* name );

	//! Parses a variable definition with the specified type and name.
	void			   ParseVariableDef( idTypeDef* type, const char* name );

	//! Parses an event definition and validates its return type and parameters against the internal event structure.
	void			   ParseEventDef( idTypeDef* type, const char* name );

	//! Parses definition statements in the compiler.
	void			   ParseDefs();

	//! Parses definitions within a namespace scope.
	void			   ParseNamespace( idVarDef* newScope );

public:
	// RB: added const
	static const opcode_t opcodes[];

	//! Constructs a new idCompiler instance and initializes its internal state.
	idCompiler();

	//! Compiles the null-terminated text and adds the resulting definitions to the program structure.
	void CompileFile( const char* text, const char* filename, bool console );
};

#endif /* !__SCRIPT_COMPILER_H__ */
