        //this UDTF takes the matrix size and returns the corresponding matrix multiplication result as table matrix.
        //compile:
        //nzudxcompile  rout.cpp --sig "rout(INT,INT, VARCHAR(ANY), VARCHAR(ANY))"  --return "TABLE(rownum int, colnum int, val double" --class "rout" --version 2 --unfenced --user username --pw password --db database
        //EXAMPLE SQL:
        //\time
        //SELECT A.ROWNUM, B.COLNUM, VAL from a, b, TABLE(rout(A.ROWNUM, B.COLNUM, A.AVAL, B.BVAL));

#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "udxinc.h"
using namespace nz::udx_ver2;
class rout : public Udtf {
private:
        bool output;public:
        rout(UdxInit *pInit) : Udtf(pInit){
        }
        static Udtf* instantiate (UdxInit *pInit);
        void newInputRow(){
                output = true;
                
        }
        DataAvailable nextOutputRow(){
                if(!output){
                        output = true;
                        return Done;
                }
                
                
                bool ANull = isArgNull(0);
                bool BNull = isArgNull(1);
                
                if (ANull || BNull) {
                        
                        return Done;
                }
                int *rownum = int32ReturnColumn(0);
                int *colnum = int32ReturnColumn(1);
                *rownum = int32Arg(0);
                *colnum = int32Arg(1);
                
                StringArg *str1 = stringArg(2);
                StringArg *str2 = stringArg(3);
                
                int i = 0;
                int len = strlen(str1->data);
                double sum = 0.0;
                while( i < len ) {
                        char a[10] = "";
                        char b[10] = "";

                        int j = 0;
                        while ( (i + j) < len && str1->data[i + j] != ';' && str2->data[i + j] != ';') {
                                a[j] = str1->data[i + j];
                                b[j] = str2->data[i + j];
                                j++;
                        }
                        sum = sum + atof(a) * atof(b);
                        i = 1 + i + j;
                }
                
                *doubleReturnColumn(2) = sum;
                output = false;
                return MoreData;
        }


};
Udtf* rout::instantiate(UdxInit *pInit){
        return new rout(pInit);
}