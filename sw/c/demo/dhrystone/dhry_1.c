/*
 ****************************************************************************
 *
 *                   "DHRYSTONE" Benchmark Program
 *                   -----------------------------
 *                                                                            
 *  Version:    C, Version 2.1
 *                                                                            
 *  File:       dhry_1.c (part 2 of 3)
 *
 *  Date:       May 25, 1988
 *
 *  Author:     Reinhold P. Weicker
 *
 ****************************************************************************
 */

#include "dhry.h"
#include "demo_system.h"
#include "timer.h"

#ifndef REG
#define REG
#endif

/* Global Variables: */
Rec_Pointer     Ptr_Glob,
                Next_Ptr_Glob;
int             Int_Glob;
Boolean         Bool_Glob;
char            Ch_1_Glob,
                Ch_2_Glob;
int             Arr_1_Glob [50];
Arr_2_Dim       Arr_2_Glob;

/* variables for time measurement: */
struct tms      time_info;
long            Begin_Time,
                End_Time,
                Begin_Insn,
                End_Insn;
float           Microseconds,
                Dhrystones_Per_Second;

int Number_Of_Runs;

/* end of variables for time measurement */

int dhry_main(void) {
        One_Fifty       Int_1_Loc;
        One_Fifty       Int_2_Loc;
        One_Fifty       Int_3_Loc;
        char            Ch_Index;
        Enumeration     Enum_Loc;
        Str_30          Str_1_Loc;
        Str_30          Str_2_Loc;
        int             Run_Index;
        
        /* Initialize variables */
        Next_Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));
        Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));
        
        Ptr_Glob->Ptr_Comp                    = Next_Ptr_Glob;
        Ptr_Glob->Discr                       = Ident_1;
        Ptr_Glob->variant.var_1.Enum_Comp     = Ident_3;
        Ptr_Glob->variant.var_1.Int_Comp      = 40;
        strcpy (Ptr_Glob->variant.var_1.Str_Comp, 
                "DHRYSTONE PROGRAM, SOME STRING");
        strcpy (Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");
        
        Arr_2_Glob [8][7] = 10;
        
        /* Start timing */
        // Begin_Time = (long) get_elapsed_time();
        Begin_Time = get_cycle_count();
        
        /* Main benchmark loop */
        for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index) {
            Proc_5();
            Proc_4();
            Int_1_Loc = 2;
            Int_2_Loc = 3;
            strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
            Enum_Loc = Ident_2;
            Bool_Glob = ! Func_2 (Str_1_Loc, Str_2_Loc);
            while (Int_1_Loc < Int_2_Loc) {
                Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
                Proc_7 (Int_1_Loc, Int_2_Loc, &Int_3_Loc);
                Int_1_Loc += 1;
            }
            Proc_8 (Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
            Proc_1 (Ptr_Glob);
            for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index) {
                if (Enum_Loc == Func_1 (Ch_Index, 'C')) {
                    Proc_6 (Ident_1, &Enum_Loc);
                    strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");
                    Int_2_Loc = Run_Index;
                    Int_Glob = Run_Index;
                }
            }
            Int_2_Loc = Int_2_Loc * Int_1_Loc;
            Int_1_Loc = Int_2_Loc / Int_3_Loc;
            Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
            Proc_2 (&Int_1_Loc);
        }
        
        /* Stop timing */
        // End_Time = (long) get_elapsed_time();
        End_Time = get_cycle_count();
        
        return 0;  /* Success */
}

void Proc_1(Rec_Pointer Ptr_Val_Par) {
  REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;

                                        /* == Ptr_Glob_Next */
  /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
  /* corresponds to "rename" in Ada, "with" in Pascal           */
  
  structassign (*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob); 
  Ptr_Val_Par->variant.var_1.Int_Comp = 5;
  Next_Record->variant.var_1.Int_Comp 
        = Ptr_Val_Par->variant.var_1.Int_Comp;
  Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
  Proc_3 (&Next_Record->Ptr_Comp);
    /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp 
                        == Ptr_Glob->Ptr_Comp */
  if (Next_Record->Discr == Ident_1)
    /* then, executed */
  {
    Next_Record->variant.var_1.Int_Comp = 6;
    Proc_6 (Ptr_Val_Par->variant.var_1.Enum_Comp, 
           &Next_Record->variant.var_1.Enum_Comp);
    Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
    Proc_7 (Next_Record->variant.var_1.Int_Comp, 10, 
           &Next_Record->variant.var_1.Int_Comp);
  }
  else /* not executed */
    structassign (*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
} /* Proc_1 */


void Proc_2 (Int_Par_Ref)
/******************/
    /* executed once */
    /* *Int_Par_Ref == 1, becomes 4 */

One_Fifty   *Int_Par_Ref;
{
  One_Fifty  Int_Loc;  
  Enumeration   Enum_Loc;

  Int_Loc = *Int_Par_Ref + 10;
  do /* executed once */
    if (Ch_1_Glob == 'A')
      /* then, executed */
    {
      Int_Loc -= 1;
      *Int_Par_Ref = Int_Loc - Int_Glob;
      Enum_Loc = Ident_1;
    } /* if */
  while (Enum_Loc != Ident_1); /* true */
} /* Proc_2 */


void Proc_3 (Ptr_Ref_Par)
/******************/
    /* executed once */
    /* Ptr_Ref_Par becomes Ptr_Glob */

Rec_Pointer *Ptr_Ref_Par;

{
  if (Ptr_Glob != Null)
    /* then, executed */
    *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
  Proc_7 (10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
} /* Proc_3 */


void Proc_4 () /* without parameters */
/*******/
    /* executed once */
{
  Boolean Bool_Loc;

  Bool_Loc = Ch_1_Glob == 'A';
  Bool_Glob = Bool_Loc | Bool_Glob;
  Ch_2_Glob = 'B';
} /* Proc_4 */


void Proc_5 () /* without parameters */
/*******/
    /* executed once */
{
  Ch_1_Glob = 'A';
  Bool_Glob = false;
} /* Proc_5 */

