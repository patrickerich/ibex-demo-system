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
uint64_t        Begin_Time,
                End_Time;
float           Microseconds,
                Dhrystones_Per_Second;

int main() {
        One_Fifty       Int_1_Loc;
        One_Fifty       Int_2_Loc;
        One_Fifty       Int_3_Loc;
        char            Ch_Index;
        Enumeration     Enum_Loc;
        Str_30          Str_1_Loc;
        Str_30          Str_2_Loc;
        int             Run_Index;
        int             Number_Of_Runs;

        /* UART initialization - add these lines */
        uart_enable_rx_int();
    
        /* Timer initialization */
        timer_init();
 
        /* Initializations */
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
 
        puts("\n");
        puts("Dhrystone Benchmark, Version 2.1 (Language: C)\n");
        puts("\n");
 
        /* Set a fixed number of runs - modify as needed */
        Number_Of_Runs = 10000;
        
        puts("Executing ");
        putdec(Number_Of_Runs);
        puts(" runs through Dhrystone\n");
 
        /***************/
        /* Start timer */
        /***************/
        Begin_Time = timer_read();
 
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
 
        /**************/
        /* Stop timer */
        /**************/
        End_Time = timer_read();
 
        puts("Execution complete\n");

        /* Display results */
        puts("Elapsed timer ticks: ");
        putdec((unsigned int)(End_Time - Begin_Time));
        puts("\n");

        float dmips = ((float)Number_Of_Runs) / 
                      ((float)(End_Time - Begin_Time) * 1757.0 / 1000000.0);
                      
        /* Print DMIPS/MHz */
        puts("DMIPS/MHz: ");
        print_float3(dmips);
        puts("\n");

        // /* Output validation values */
        // puts("Final values of the variables used in the benchmark:\n");
        // puts("Int_Glob:            ");
        // putdec(Int_Glob);
        // puts("\n");
        // puts("Bool_Glob:           ");
        // putdec(Bool_Glob);
        // puts("\n");
        
        /* Continue with more validation outputs if desired */
        
        puts("\nBenchmark complete\n");
        
        // while(1) { /* Loop forever */ }

        // Simple delay to allow UART transmission to complete
        volatile uint32_t i;
        for (i = 0; i < 100000; i++) {
            asm volatile("nop");
        }
        // Then halt the simulation
        sim_halt();
}

/* Now include all the Proc_X function implementations */
void Proc_1(Rec_Pointer Ptr_Val_Par) {
  REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;
  
  structassign (*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob); 
  Ptr_Val_Par->variant.var_1.Int_Comp = 5;
  Next_Record->variant.var_1.Int_Comp 
        = Ptr_Val_Par->variant.var_1.Int_Comp;
  Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
  Proc_3 (&Next_Record->Ptr_Comp);
  if (Next_Record->Discr == Ident_1)
  {
    Next_Record->variant.var_1.Int_Comp = 6;
    Proc_6 (Ptr_Val_Par->variant.var_1.Enum_Comp, 
           &Next_Record->variant.var_1.Enum_Comp);
    Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
    Proc_7 (Next_Record->variant.var_1.Int_Comp, 10, 
           &Next_Record->variant.var_1.Int_Comp);
  }
  else
    structassign (*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
}

void Proc_2 (Int_Par_Ref)
One_Fifty   *Int_Par_Ref;
{
  One_Fifty  Int_Loc;  
  Enumeration   Enum_Loc;

  Int_Loc = *Int_Par_Ref + 10;
  do
    if (Ch_1_Glob == 'A')
    {
      Int_Loc -= 1;
      *Int_Par_Ref = Int_Loc - Int_Glob;
      Enum_Loc = Ident_1;
    }
  while (Enum_Loc != Ident_1);
}

void Proc_3 (Ptr_Ref_Par)
Rec_Pointer *Ptr_Ref_Par;
{
  if (Ptr_Glob != Null)
    *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
  Proc_7 (10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
}

void Proc_4 ()
{
  Boolean Bool_Loc;

  Bool_Loc = Ch_1_Glob == 'A';
  Bool_Glob = Bool_Loc | Bool_Glob;
  Ch_2_Glob = 'B';
}

void Proc_5 ()
{
  Ch_1_Glob = 'A';
  Bool_Glob = false;
}

#ifdef  NOSTRUCTASSIGN
memcpy (d, s, l)
register char   *d;
register char   *s;
register int    l;
{
        while (l--) *d++ = *s++;
}
#endif
