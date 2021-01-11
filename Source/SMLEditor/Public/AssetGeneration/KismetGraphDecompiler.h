#pragma once
#include "CoreMinimal.h"
#include "KismetIntermediateFormat.h"
#include "EdGraphSchema_K2.h"

class SML_API FKismetGraphDecompiler {
public:
    /** Constructs decompiler object for provided function and graph */
    FKismetGraphDecompiler(UFunction* Function, UEdGraph* Graph);

    /** Initializes decompiler with the compiled kismet statement list */
    void Initialize(const TArray<TSharedPtr<FKismetCompiledStatement>>& Statements);

    
private:
    UEdGraphNode* CreateMakeMapNode();
    UEdGraphNode* CreateMakeSetNode();
    UEdGraphNode* CreateMakeArrayNode();

    UEdGraphNode* CreateDynamicCastNode(UEdGraphPin* PreviousNodeExecOutput);
    UEdGraphNode* CreateAssignmentNode(UEdGraphPin* PreviousNodeExecOutput);
    UEdGraphNode* CreateMakeStructNode(TSharedPtr<FKismetTerminal> StructTerminal);
    UEdGraphNode* CreateSetFieldsInStructNode(TSharedPtr<FKismetTerminal> StructTerminal, UEdGraphPin* PreviousNodeExecOutput);
    UEdGraphNode* CreateCopyNode();
    UEdGraphNode* CreateDelegateSetNode(UEdGraphPin* PreviousNodeExecOutput);
    
    UEdGraphNode* GenerateNodeForStatement(int32& StatementIndex, UEdGraphPin* PreviousNodeExecOutput);
    UEdGraphPin* GenerateCodeForTerminalAndConnectItToPin(UEdGraphPin* ResultPin, TSharedPtr<FKismetTerminal> Terminal);
    void ConnectGraphPins(UEdGraphPin* FirstPin, UEdGraphPin* SecondPin);
    FString ConvertTerminalToQualifiedName(const TSharedPtr<FKismetTerminal> Terminal);

    /** Returns true if local variable in question was created manually by the user and not auto generated by kismet compiler */
    bool IsUserCreatedLocalVariable(TSharedPtr<FKismetTerminal> Terminal);

    /** Returns true if variable in question is a local variable, even if it is located in class but still considered local */
    bool IsLocalVariable(TSharedPtr<FKismetTerminal> Terminal);

    /** Retrieves current statement from the list, possibly with an offset. Does not advance reader index */
    FORCEINLINE TSharedPtr<FKismetCompiledStatement> PeekStatement(int32 Offset = 0) {
        return CompiledStatements[CurrentStatementIndex + Offset];
    }

    /** Retrieves current statement from the list and advances current statement index */
    FORCEINLINE TSharedPtr<FKismetCompiledStatement> PopStatement() {
        return CompiledStatements[CurrentStatementIndex++];
    }

    /** Retrieves first kismet graph node with provided class and asserts if it's not found */
    template<typename T>
    FORCEINLINE T* FindFirstGraphNodeOfClass() {
        TArray<T*> FoundKismetNodes;
        EditorGraph->GetNodesOfClass<T>(FoundKismetNodes);
        check(FoundKismetNodes.Num());
        return FoundKismetNodes[0];
    }
    
    /** Function we are currently reconstructing code for */
    UFunction* Function;
    /** Graph containing source code for edited function */
    UEdGraph* EditorGraph;
    
    /** Collection of statements we are decompiling */
    TArray<TSharedPtr<FKismetCompiledStatement>> CompiledStatements;
    int32 CurrentStatementIndex;

    /** Map of intermediate variable names to node outputs which caused their generation */
    TMap<FString, UEdGraphPin*> IntermediateVariableHandles;

    /** After code generation, pin specified as the key will be automatically connected to the input exec pin of the value node */
    TMap<UEdGraphPin*, TSharedPtr<FKismetCompiledStatement>> ExecPinPatchUpMap;
    
};