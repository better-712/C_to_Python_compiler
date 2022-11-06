# SUSTech Programming Language(SPL) project 1

1. Flex for Scanner
2. Bison for Parser



##### Usage

``` shell
sudo apt-get install flex bison # install required package
make # Generate splc executable file in ./bin
./bin/splc test/test_1_o01.spl # Generate test/test_1_o01.out
```

If the input program is free from lexical or syntax error, the output would be the syntax tree like

```
Program (1)
  ExtDefList (1)
    ExtDef (1)
      Specifier (1)
        TYPE: int
      FunDec (1)
        ID: test_1_r01
        LP
        VarList (1)
          ParamDec (1)
            Specifier (1)
              TYPE: int
            VarDec (1)
              ID: a
          COMMA
          VarList (1)
            ParamDec (1)
              Specifier (1)
                TYPE: int
              VarDec (1)
                ID: b
        RP
      CompSt (2)
        LC
        StmtList (3)
          Stmt (3)
            Exp (3)
              Exp (3)
                ID: c
              ASSIGN
              Exp (3)
                CHAR: 'c'
            SEMI
          StmtList (4)
            Stmt (4)
              IF
              LP
              Exp (4)
                Exp (4)
                  ID: a
                GT
                Exp (4)
                  ID: b
              RP
              Stmt (5)
                CompSt (5)
                  LC
                  StmtList (6)
                    Stmt (6)
                      RETURN
                      Exp (6)
                        ID: a
                      SEMI
                  RC
              ELSE
              Stmt (9)
                CompSt (9)
                  LC
                  StmtList (10)
                    Stmt (10)
                      RETURN
                      Exp (10)
                        ID: b
                      SEMI
                  RC
        RC
```

Otherwise, error message would be provided:

```
Error type A at Line 3: unknown lexeme 0x77G
Error type A at Line 4: unknown lexeme 0xCS
Error type A at Line 7: unknown lexeme '\x6u'
Error type A at Line 8: unknown lexeme '\x910'
```




### Acknowledgement
1. Thanks C++ Flex/Bison template from https://github.com/jonathan-beard/simple_wc_example
2. Thanks Bison example from https://github.com/google/classp/blob/master/tests/example.y