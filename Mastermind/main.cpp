#include <iostream>
#include <map>

using namespace std;

int main()
{
    string palavra = "canas";
    string palavra2;
    string check[palavra.size()]; //guarda os valores de retorno B,W ou _
    int play=1;//numero da jogada, termina ao fim de 10
    int match=0;//numero de letras certas, termina quando for igual ao tamanho da palavra
    map<char,int> counter;//guarda a quantidade de cada letra. ex: canas - c=1; a=2 etc.

    while(play<11 && match!=palavra.size()){//cada iteraçao corresponde a uma jogada
        //limpa todos os dados
        counter.clear();
        for(int i=0;i<palavra.size();i++){
            check[i]="";
        }
        match=0;

        //print da palavra codificada(em _)
        for(int i=0;i<palavra.size();i++){
            cout << "_ ";
        }
        cout << endl;
        //recebe a tentativa
        cin >> palavra2;
        if(palavra.size()==palavra2.size()){//se o tamanho for diferente nao aceita
            //actualiza o counter com a quantidade de cada letra
            for(int i=0;i<palavra.size();i++){
                if(!counter.empty()){
                    if(counter.find(palavra[i])==counter.end()){//a letra ainda nao foi inicializada no counter
                        counter[palavra[i]]=1;
                    }else{//a letra ja existe
                        counter[palavra[i]]+=1;
                        }
                }else{//counter vazio
                    counter[palavra[i]]=1;
                }
            }
            //procura letras ja nao posiçao certa; actualiza o check e o counter e incrementa o match
            for(int i=0;i<palavra.size();i++){
                if(palavra[i]==palavra2[i]){
                    check[i]="B";
                    counter[palavra[i]]--;
                    match++;
                }
            }
            if(match<palavra.size()){
                //verifica o resto das letras (fora da posiçao e nao existentes
                for(int i=0;i<palavra.size();i++){
                    for( int j=0;j<palavra2.size();j++){
                         if(i!=j&&palavra[i]==palavra2[j]&&counter[palavra[i]]>0&&check[j]!="B"){//só conta como W se nao exceder o nr de letras da palavra
                            check[j]="W";
                            counter[palavra[i]]=counter[palavra[i]]-1;
                         }else if(check[j]==""){//no caso de exceder o nr de letras, por exemplo, 3º a em canas é tratado como _ apesar de pertencer à palavra
                             check[j]="_";
                         }
                    }
                }
            }
            //print da jogada
            for(int i=0;i<palavra.size();i++){
                cout << check[i];
            }
            cout<< endl;

        }
    //fim da jogada
    play++;
    }
    //fim do jogo
    if(play==11){
        cout<< "Game Over\n";
    }else {
        cout << "Congratulations\n";
    }
    return 0;
}
