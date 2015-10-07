        // This UDTF takes in matrix size and returns table with the same size for verification.
        // compile:
        // nzudxcompile --sig "Bin(int, int)" --return "TABLE(COLNUM INT, BVAL VARCHAR(15000))" --class "Bin" --version 2 --unfenced --user username --pw password --db Database
        // Example SQL:
        //\time
        // CREATE TABLE A as select * from table(Bin(0, 10)) distribute on (COLNUM);
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "udxinc.h"
#include "time.h"
using namespace nz::udx_ver2;
class Bin : public Udtf {
private:
        int m_current;
        int m_stop;
        int flag;
public:
        Bin (UdxInit *pInit) : Udtf(pInit){
        }
        static Udtf* instantiate(UdxInit *pInit);
        void newInputRow() {
                m_current = 1;
                m_stop = int32Arg(1);
                if (int32Arg(1) == 1) {
                        flag = 1;
                }
                else
                        flag = 0; // for verification
                
        }
        DataAvailable nextOutputRow() {
                float val;
                if ( m_current > m_stop ){
                        return Done;
                }

                int *rn = int32ReturnColumn(0);
                *rn = m_current;
                
                StringReturn *str = stringReturnColumn(1);
                
                str->data[0] = '\0';
                for ( int j = 1; j < m_stop; j++) {
                        if (flag == 0) {
                                val = 1.0;
                        }
                        else{
                                srand(time(NULL));
                                val = (float)(rand()%2);
                        }
                        
                        if ( j != 1) {
                                strcat(str->data, ";");
                        }
                        char buffer[100] = "";
                        sprintf(buffer, "%.1f", val);
                        strncat(str->data, buffer, 3);
                }
                
                m_current++;
                return MoreData;
        }
};
Udtf* Bin::instantiate(UdxInit *pInit) {
        return new Bin(pInit);
}