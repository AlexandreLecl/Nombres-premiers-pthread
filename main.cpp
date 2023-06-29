#include "main.h"

//Global variables
std::vector<mpz_class> listNumber;
pthread_mutex_t mutexNumbers = PTHREAD_MUTEX_INITIALIZER;
std::string lineNumbers[2];
pthread_t* listThread;
struct arg_struct* listarguments;

void* findFirstNumbers(void *arguments){
    //Find first numbers in an interval
    struct arg_struct *args = (struct arg_struct *)arguments;
    int n,q;
    const char* infChar=(args->inf).c_str(); //Necessary to use const char* and not string for mpz_set_str
    const char* supChar=(args->sup).c_str();
    mpz_t inf,temp,sup;
    mpz_t PrimeCandidate;
    mpz_class primeCandidateMPZCLASS;   //Obligatory to use a vector for listNumber
    std::vector<mpz_class> listNumberTemp;
    
    mpz_init(PrimeCandidate); // Important without could provoc seg fault (sometimes)
    mpz_init(inf);
    mpz_init(temp);
    mpz_init(sup);

    mpz_set_str(inf,infChar, BASE);
    mpz_set_str(sup,supChar, BASE);

    if(mpz_cmp(sup,inf)<0){
        // Invert sup and inf if sup<inf
        mpz_set(temp,sup);
        mpz_set(sup,inf);
        mpz_set(inf,temp);
    }

    mpz_set(PrimeCandidate,inf);


    while(mpz_cmp(PrimeCandidate,sup)<=0){     //Equivalent to PrimeCandidate <= sup
        if(mpz_probab_prime_p(PrimeCandidate,MILLERRABINPARAM)){
            primeCandidateMPZCLASS=mpz_class(PrimeCandidate);
            listNumberTemp.push_back(primeCandidateMPZCLASS);
        }
        mpz_add_ui(PrimeCandidate,PrimeCandidate,1);// PrimeCandidate++
    }

    pthread_mutex_lock(&mutexNumbers);
    // Insert prime numbers to the correct emplacement
    for(q=0;q<listNumberTemp.size();q++){
        if(listNumber.empty()) listNumber.push_back(listNumberTemp[q]);

        else{
            if(listNumber.back()<listNumberTemp[q]) listNumber.push_back(listNumberTemp[q]);
            else{
                n=0;
                while( (listNumber[n]<listNumberTemp[q]) && (listNumber[n]!=listNumber.back()) ) n++;

                if(listNumber[n]!=listNumberTemp[q]){
                    //If the number is not already in the list
                    listNumber.insert(listNumber.begin()+n,listNumberTemp[q]);
                }
            }
        }
    }
    pthread_mutex_unlock(&mutexNumbers);
    pthread_exit(NULL);
}

int readLine(char* line){
    //Extract 2 numbers from a line
    int i,j;
    char* end;
    std::string infStr,supStr;
    std::string lineStr(line);
    i=0;
    
    while(line[i]!=' ') i++;
    infStr=lineStr.substr(0,i);
    
    j=i;
    while(line[j+1]!='\n') j++;
    supStr=lineStr.substr(i+1,j);

    lineNumbers[0]=infStr;
    lineNumbers[1]=supStr;
    return 0;
}


int main(int argc,char *argv[]){
    if(argc<2) std::cout <<"Not enough arguments, 2 required\n" <<std::endl;
    else{
        int nbrThreads=atoi(argv[1]);
        const char * filename=argv[2];
        int k=1;
        //int test=1;
        int i,j,m,p;
        listThread=(pthread_t*)malloc(nbrThreads*sizeof(pthread_t)); 
        listarguments=(struct arg_struct*)malloc(nbrThreads*sizeof(struct arg_struct)); 

        char line[MAXCHARSIZE];
        char* check;
        FILE * file = fopen(filename,"r");
        if(file==NULL) std::cout <<"Not able to open the file\n" <<std::endl;
        else{
            
            Chrono chrono = Chrono();   //Chrono autostart
            check=fgets(line, MAXCHARSIZE, file);
            while(check!=NULL){
                
                readLine(line);  
                listarguments[k-1].inf=lineNumbers[0];
                listarguments[k-1].sup=lineNumbers[1];
                pthread_create(&listThread[k-1],NULL,findFirstNumbers, (void*)&listarguments[k-1]);
                k++;

                if(k>nbrThreads){
                    
                    for(p=1;p<=nbrThreads;p++){
                        //printf("Join n° %d\n",test);
                        //test++;
                        pthread_join(listThread[p-1],NULL); //Wait for threads completion
                    }
                    k=1;
                }
                check=fgets(line, MAXCHARSIZE, file);
            }
            
            
            //We make sure to wait all threads
            for(p=1;p<=nbrThreads;p++){
                //printf("Join type 2\n");
                pthread_join(listThread[p-1],NULL);
            }
            double time =chrono.get();   
            chrono.pause();     //End of chrono

            fclose(file);
            
            for(m=0;m<listNumber.size();m++){
                std::cout << listNumber[m] << "\n";
            }

            fprintf(stderr,"%f s\n",time);
        }
    }
    return 0;
}