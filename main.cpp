#include<bits/stdc++.h>
#include<pthread.h>
#include<time.h>
using namespace std;
int n1,m1,n2,m2,z[500][500],x[500][500],y[500][500];

struct dual{
    int r;
    int c;
};

void *Mul(void *p)
{
    void *ret;
    dual *ip = (dual *)p;
    int i = ip->r;
    int j = ip->c;
    for(int k=0;k<m1;k++)
        z[i][j] += x[i][k]*y[k][j];
    free(p);
    return ret;
}

void *rMul(void *p)
{
    void *ret;
    int i = *((int *)p);

    for(int j=0;j<m2;j++)
        for(int k=0;k<m1;k++)
            z[i][j] += x[i][k]*y[k][j];
    free(p);
    return ret;
}

int main()
{
    printf("ENTER FILE NAME : ");
    char name[1000]; scanf("%s",name);
    if(!freopen(name, "r", stdin)){
        cout << "File NOT Found" << endl;
        return 0;
    }
    freopen ("Output_fil.txt","w",stdout);
    cin >> n1 >> m1;
    for(int i=0;i<n1;i++)
        for(int j=0;j<m1;j++)
            cin >> x[i][j];
    cin >> n2 >> m2;
    for(int i=0;i<n2;i++)
        for(int j=0;j<m2;j++)
            cin >> y[i][j];

    if(n2!=m1)
    {
        cout << "MATRIX MULTIPLICATION IS NOT POSSIBLLE" << endl;
        return 0;

    }

    clock_t start = clock();

    /* here, do your time-consuming job */
    pthread_t th[n1*m2] , th2[n1];
    for(int i=0;i<n1;i++)
        for(int j=0;j<m2;j++)
        {
            dual *cell = (dual *)malloc(sizeof(dual));
            cell->r = i;
            cell->c = j;
            if(pthread_create(&th[i*m2+j],NULL,&Mul,cell))
            {
                cout << "error creating thread" << endl;
                return 0;
            }
        }
    for(int i=0;i<n1;i++)
        for(int j=0;j<m2;j++)
            pthread_join(th[i*m2+j],NULL);
    cout << "Multiplication By Element" << endl;
    for(int i=0;i<n1;i++ , cout << endl)
        for(int j=0;j<m2;j++,cout << "\t")
            cout << z[i][j];

    clock_t finish = clock();
    double Total_time = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "END 1\t" << Total_time << "s" << endl;

    for(int i=0;i<n1;i++)
        for(int j=0;j<m2;j++)
            z[i][j]=0;

    start = clock();

    for(int i=0;i<n1;i++)
    {
        int *row = (int *)malloc(sizeof(int));
        *row = i;
        if(pthread_create(&th2[i],NULL,&rMul,row))
        {
            cout << "error creating thread" << endl;
            return 0;
        }
    }
    for(int i=0;i<n1;i++)
        pthread_join(th2[i],NULL);

    cout << "Multiplication by row" << endl;
    for(int i=0;i<n1;i++ , cout << endl)
        for(int j=0;j<m2;j++,cout << "\t")
            cout << z[i][j];

    finish = clock();
    Total_time = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "END2\t" << Total_time  << "s" << endl;
}


