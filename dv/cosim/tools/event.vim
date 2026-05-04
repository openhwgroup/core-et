if exists("b:current_syntax")
    finish
endif

let b:current_syntax = "event"

syn keyword evtTodo        contained FIXME NOTE NOTES TODO XXX
syn match   evtComment     "#.*$" contains=eventTodo,@Spell
syn keyword evtType        x8 x16 x32 x64 i8 i16 i32 i64 u8 u16 u32 u64 f32 f64 bool
syn keyword evtStruct      event
syn keyword evtConst       const
syn match   evtNumber      display '\<\d\+'
syn region  evtBracket     transparent start='\[' end=']' contains=ALLBUT,evtBracketErr
syn region  evtBlock       transparent start='{' end='}' contains=ALLBUT,evtStruct,evtBlockErr fold
syn match   evtBracketErr  ']'
syn match   evtBlockErr    '}'

hi def link evtTodo        Todo
hi def link evtComment     Comment
hi def link evtType        Type
hi def link evtStruct      Structure
hi def link evtConst       StorageClass
hi def link evtNumber      Number
hi def link evtBracketErr  Error
hi def link evtBlockErr    Error
