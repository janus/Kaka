#include <stdio.h>
  #include <string.h>
  #include <stdlib.h>

    /* (C) Copyright 2015, Emeka N. Freely we received and free do we give
      *
      *   Evaluate an expression pointed to by str. Expressions evaluate
      *   right to left unless parenthesis are present. But it obeys precedent rule multiplication and division are higher.
      *  Valid operators are
      *   * + - / for multiply add, subtract and divide. The expression must
      *   be formed from the character set { 0123456789+-*()/ }. White
      *   space is not allowed.
      *
      *   <expr>      ::=    <factor>
      *                    | <factor> * <expr>
      *                    | <factor> / <expr>
      *                    | <factor> + <expr>
      *                    | <factor> - <expr>
      *
      *   <factor>    ::=     ( <expr> )
      *                    | -( <expr> )
      *                    |  <constant>
      *                    | -<constant>
      *
      *   <constant>  ::=  A string of ASCII chars in the range '0'-'9'.
      *
      *---------------------------------------------------------------
      *  Global variables:
      *  Examples:
      *  ?=  78 op 37
      * op could be any of {- , + , * /}
      * assignment is also possible
      *  ?= xval = 40 - 21
      *  And you can have variables in your expression but it should not be the first operand
      *  ?= 100 + xval
      */

     static char *Str ;   /* Current position in string being parsed    */
     static int  Error;   /* # of errors found so far                   */
     static int assignStatus = 0;
     struct env
     {
         char* keyValue;
         int rValue;
         struct env* next;
     };
     struct env* head;  //The head of the list

  void initEnv()
  {
       head = (struct env*)malloc(sizeof(struct env));

       if (!head)
        {
            printf("Not enough memory to create new env");
            exit(0);
        }

        head->next = 0;
  }

 int lookup(struct env* startptr, const char* keyStr )
 {
     struct env* temp;
     temp = startptr->next;

     while(temp)
     {
         if (strcmp((const char*)temp->keyValue, keyStr) == 0)
            return temp->rValue;
        //Traversing a list looking for a match
        temp = temp->next;
     }

     printf("****----ERROR----Using unassigned variable");
     exit(0);
 }

  void  envInsert(char* lVal, int rVal)
  {

       //Insert to list which is a global environment
        struct env* temp;
        temp = (struct env*)malloc(sizeof(struct env));

        if (!temp)
        {
            printf("Not enough memory to create new env");
            exit(0);
        }
        temp->rValue = rVal;
        temp->keyValue = lVal;
        temp->next = 0;
        if(!head->next)
        {
            head->next = temp;
        }
        else {
        temp->next = head->next;
        head->next = temp;

        }

  }

  char* envValue(size_t nsize, char*  startKey )
  {
      char* temp = (char*)malloc((nsize+1)*sizeof(char));
      if (!temp)
      {
         printf("No memory to work with");
         exit(0);
      }
      strncpy(temp, (const char*) startKey, nsize );
      temp[nsize] = '\0';

      return temp;


  }



     int     parse( char  *expression, int *err )
     {
            /* Return the value of "expression" or 0 if any errors were
              * found in the string. "*Err" is set to the number of errors.
             * "Parse" is the "access routine" for expr(). By using it you
              * need not know about any of the global vars used by expr()
              * Edited from source .
              */

             register int        rval;

             Error = 0;
             Str = expression;
             rval = stat();
             //if (assignStatus) {return; }
             return( (*err = Error) ? 0 : rval );
    }

// The beginning of recursive descent parser
int stat()
{
    while(isspace(*Str)|| *Str == ' '){Str++;}
    if(isalpha(*Str))
    {
        char* p = Str;
        int b;
        char* keyValue;
        int exprResult;

        b = constantp();
        keyValue = envValue((size_t)b , p );
        while(isspace(*Str)|| *Str == ' '){Str++;}
        if(*Str == '=')
        {
          Str++;
          exprResult = expr();

        }

        else
        {
         printf("ERROR=== missed the sign = (assignment sign)");
         exit(0);
        }
        assignStatus = 1;
        envInsert(keyValue, exprResult);
        return exprResult;
    }

    assignStatus = 0;
    return expr();
}
       int expr()
    {
            int    lval;


            while(isspace(*Str)|| *Str == ' '){Str++;}
            lval = term();
            while(isspace(*Str)|| *Str == ' '){Str++;}
            switch (*Str)
           {
            case '+':  Str++;  lval += expr(); break;
            case '-':  Str++; lval -= expr();  break;
            default :  break;
            }
            return( lval );
    }

    /*------------------------------------------------------------------*/
    int term ()
    {
        int rval = 0;
        while(isspace(*Str)|| *Str == ' '){Str++;}
        rval = factor();
        while(isspace(*Str)|| *Str == ' '){Str++;}
        switch(*Str)
        {
        case '*': Str++;  rval *= term(); break;
        case '/' :  Str++;  rval /= term(); break;
        default: break;
        }

        return rval;
    }
    int factor()
    {
            int    rval = 0 , sign = 1 ;

            if ( *Str == '-' )
           {
                    sign = -1 ;
                    Str++;
           }

            if ( *Str != '(' ){
                    rval = constant();
                }
            else
           {
                    Str++;
                   rval = expr();

                  if ( *Str ==')')
                           Str++;
                   else
                    {
                           printf("Mis-matched parenthesis\n");
                            Error++ ;
                    }
           }

            return (rval * sign);
    }

    /*------------------------------------------------------------------*/
     int constant()
    {
            int    rval = 0;
            int  countChar = 0;
            char* tStr;
            char* kVal;
            int index = 0;
            int vValue;

            if (isalpha(*Str))
            {
             tStr = Str;
                while ( *Str && isalpha( *Str ))
                    {
                    index++;
                    Str++;
                    }
                    kVal = (char *)malloc((index + 1)*sizeof(char));
                    strncpy(kVal, (const char*)tStr, index);
                    kVal[index] = '\0';
                    rval = lookup(head, (const char* )kVal);
                    return rval;
            }

           while ( *Str && isdigit(*Str) )
            {
                     rval = (rval * 10) + (*Str - '0');
                     Str++;
            }

            return( rval );
    }

   int  constantp()
    {
            char* lval = 0 ;
            int rtemp = 0;
            if( isdigit( *Str ))
                    Error++;

           while ( *Str && isalpha(*Str) )
            {
                     rtemp++;
                     Str++;
            }

            //lval =  (char*) (Str - 1);

            return rtemp ;
    }
     /*--------------------------------------------------------------*/
  //   #ifdef DEBUG
    int  main(int argc, char  **argv)
       {
             /*     Routine to exercise the expression parser. If an
              *     expression is given on the command line it is
              *     evaluated and the result is printed, otherwise
              *     expressions are fetched from stdin (one per line)
              *     and evaluated. The program will return -1 to the
              *     shell on a syntax error, 0 if it's in interactive
              *     mode, otherwise it returns the result of  the
              *     evaluation.
              *     From the source
              */

            char buf[133], *bp = buf;
            int err, rval;
            struct env* tempHolder;
             if( argc > 2 )
             {
                     fprintf(stderr, "Usage: expr [<expression>]");
                     exit( -1 );
             }
             if( argc > 1 )
             {
                     rval = parse( argv[1], &err );
                     printf(err ? "*** ERROR ***" : "%d", rval );
                     exit( rval );
             }
          printf("Enter expression or <CR> to exit program\n");
          initEnv();
         while( 1 )
            {
                    printf("?= ");
                  if(gets(buf) == NULL || !*buf )
                         exit(0);
                    rval= parse(buf, &err);
                    if( err )
                             printf("*** ERROR ***\n");
                     else
                     {
                        // if (assignStatus)
                         //printf("Value assigned; return 0")
                        // else
                         printf("%s: %d\n", buf, rval);
                     }
            }
            while(head->next)
            {
                tempHolder = head->next;
                if (head->keyValue)
                    free(head->keyValue);
                free(head);
                head = tempHolder;
            }
            return 0;
     }

   //  #endif
     /*------------------------------------------------------------------*/
