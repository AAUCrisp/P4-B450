/*
  Low level precision floating point lib
  Author: Henrik Schiøler
*/

//low precision floating pt type
typedef struct myfloat
{
        signed char mantissa;
        signed char exponent;
} myfloat_type;


//convert from double to low precision type
void doub2mydouble(double arg, myfloat_type *res)
{  
   int exponent;
   double temp;
   exponent = ceil(log(abs(arg))/log(2)); //base 2 logarithm
   temp=arg*pow(2,7-exponent);
   res->mantissa = (signed char)temp;
   res->exponent = exponent-7;
}

//convert from low precision type to double
double myfloat2double(myfloat_type *arg1)
{
  double res = (double)(arg1->mantissa) * pow(2,arg1->exponent);
   return res;
}

//multiply to low precision types
void mult_float(myfloat_type *arg1,myfloat_type *arg2,myfloat_type *result)
{
  int temp;
  unsigned char sign;
  
  sign=0x80 & ((unsigned char)arg1-> mantissa ^ (unsigned char)arg2-> mantissa); //find sign of result 
  
  char i=0;
   temp = (int)(arg1-> mantissa) * (int)(arg2-> mantissa);
   
   temp = temp & 0x7f00; //take away sign from product
  
   while(abs(temp)>128)
   {
      i++;
      temp=temp>>1;
   }
   
   result->mantissa = (unsigned char) temp;
   
   result->mantissa = result->mantissa | sign; //add recorded sign
   
   result->exponent = arg1->exponent + arg2->exponent + i;
   
}
  


double da[100];
double da2[100];
myfloat_type mda[100];
double mda2[100];
int on = 0;
double mean = 0;
double mean2 = 0;
double a=1.001;
myfloat_type f1;
unsigned long start = 0;
unsigned long finish = 0;
double delta = 0;
double delta2 = 0;
myfloat_type f;
double timeDif = 0;

void function1(){
    for(int i = 0;i<100;i++){ // Do the da and mda stuff
        Serial.print("da: ");
        Serial.println(da[i]);
        Serial.print("Manitissa: ");
        Serial.println(mda[i].mantissa);
        Serial.print("Exponent");
        Serial.println(mda[i].exponent);
        Serial.print("mda: ");
        Serial.println(myfloat2double(&mda[i]));
        Serial.print("Relative error: ");
        Serial.println((abs(da[i])-abs(myfloat2double(&mda[i])))/abs(da[i]),10);
        Serial.println("");        

        ///////////////////////// OPGAVE 3 START /////////////////////
        if(da[i] != 0){
            mean += (abs(da[i])-abs(myfloat2double(&mda[i])))/abs(da[i]);
        }
        ///////////////////////// OPGAVE 3 SLUT /////////////////////
    
    }
    for(int i = 0; i<100; i++){    //Mean2 da2 mda2 sum + print
        Serial.print("da2: ");
        Serial.println(da2[i]);
        Serial.print("mda2: ");
        Serial.println(mda2[i]);
        Serial.print("Relative error: ");
        Serial.println((abs(da2[i])-abs(mda2[i]))/abs(da2[i]),10);
        Serial.println("");
        
        ///////////////////////// OPGAVE 6 START  /////////////////////
        if(da2[i] != 0){
            mean2 += (abs(da2[i])-abs(mda2[i]))/abs(da2[i]); //Opgave 6
        }
        ///////////////////////// OPGAVE 6 SLUT   /////////////////////
    
    }
    if (true){  //Print mean relative error
        Serial.println("");
        Serial.print("Mean relative error: ");
        Serial.println(mean/100,20);
        Serial.println("");
        Serial.print("Mean2 realative error: ");
        Serial.println(mean2/100,20);
        Serial.println("");
        
        delay(5000);
    }
    
}

void function2(){
  
    ///////////////////////// OPGAVE 8 START /////////////////////
    doub2mydouble(a, &f1);
    ///////////////////////// OPGAVE 8 SLUT //////////////////////
    ///////////////////////// OPGAVE 7 START /////////////////////    
    start = micros();
    for(int i=0; i<100; i++){ //iterate a *= da[i]
        a*=da[i];
    }
    finish = micros();
    delta = finish - start;
    Serial.print("Time: ");
    Serial.println(delta);
    Serial.print("a: ");
    Serial.println(a);
    ///////////////////////// OPGAVE 7 SLUT /////////////////////
    
    ///////////////////////// OPGAVE 9 START /////////////////////
    start = micros();
    for(int i=0; i<100; i++){ //iterate mult_float
        mult_float(&f1, &mda[i], &f);
        memcpy(&f1,&f,2);
    }
    finish = micros();
    delta2 = finish - start;

    Serial.print("Time2: ");
    Serial.println(delta2);
    ///////////////////////// OPGAVE 9 SLUT /////////////////////


    ///////////////////////// OPGAVE 10 START /////////////////////
    if (delta > delta2){                                 
        Serial.print("Relative difference in time: ");    
        timeDif = (abs(delta - delta2))/abs(delta);
        Serial.println(timeDif,10);
    }
    else{
        Serial.print("Relative difference in time: ");
        timeDif = (abs(delta2 - delta))/abs(delta2);
        Serial.println(timeDif,10);
    }
    ///////////////////////// OPGAVE 10 SLUT /////////////////////
}



void init_da()
{
    for (int i = 0;i<100; i++){

        /////////////////////OPGAVE 1 START/////////////////////
        da[i] = random(-500, 501);  //Opgave 1
        da[i] = (double)da[i] / 100;
        /////////////////////OPGAVE 1 SLUT /////////////////////
        
        /////////////////////OPGAVE 4 START/////////////////////
        da2[i]=pow(da[i],2);    //Opgave 4
        /////////////////////OPGAVE 4 SLUT//////////////////////
        
        /////////////////////OPGAVE 2 START/////////////////////
        doub2mydouble(da[i], &mda[i]); //Opgave 2
        /////////////////////OPGAVE 2 SLUT//////////////////////
        
        /////////////////////OPGAVE 5 START/////////////////////
        mda2[i]=pow(myfloat2double(&mda[i]),2); //Opgave 5
        /////////////////////OPGAVE 5 SLUT//////////////////////
    }
    Serial.println("init done");
}

void setup(){
    Serial.begin(115200);
    Serial.println("Setup");
}

void loop(){
    while(on == 0){
    Serial.println("loop start");
    init_da();

    function1();
    //function2();      // For execution time taking... and... other stuff?


    Serial.println("");
    Serial.println("loop end");    
    on = 1; //make code not run again
    }
}
