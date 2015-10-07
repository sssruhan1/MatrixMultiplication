        // This UDTF takes in matrix size and returns table with the same size for verification.
        // compile:
        // nzudxcompile --sig "Ain(int, int)" --return "TABLE(ROWNUM INT, AVAL VARCHAR(15000))" --class "Ain" --version 2 --unfenced --user username --pw password --db Database
        // Example SQL:
        // \time
        // CREATE TABLE A as select * from table(Ain(0, 10)) distribute on (ROWNUM);
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "udxinc.h"
using namespace nz::udx_ver2;
class Ain : public Udtf {
private:
        int m_current;
        int m_stop;
        int flag;
public:
        Ain (UdxInit *pInit) : Udtf(pInit){
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
Udtf* Ain::instantiate(UdxInit *pInit) {
        return new Ain(pInit);
}