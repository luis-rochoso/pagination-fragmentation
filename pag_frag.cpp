#include <iostream>
#include <vector> 
#include <utility> 
using namespace std;

const int MEM_SIZE  = 8192;
const int PAGE_SIZE = 1024;

int main() {

    cout << "[Inicializando] Simulador de Paginação & Fragmentação\n" << endl;

    // Memória dividida em 8 quadros de 1024 "bytes"
    int memoria[8] = {
        PAGE_SIZE, PAGE_SIZE, PAGE_SIZE, PAGE_SIZE,
        PAGE_SIZE, PAGE_SIZE, PAGE_SIZE, PAGE_SIZE
    };

    // Seus quadros começam desocupados
    bool ocupada[8] = {0};

    // Logo, nenhum processo tá associado a nenhum quadro
    int processo_alocado[8] = {0};

    int num_processos;
    cout << "Quantos processos pra hoje, chefe?\nDigite o número: ";
    cin >> num_processos;
    // Se o processo foi alocado e o tamanho do processo
    vector<pair<bool, int>> processos;
    
    // Para cada processo...
    for (int i = 0; i < num_processos; ++i) {

        // primeiro se mostra a tabela de paginação,
        cout << "--[TABELA DE PÁGINA]--\n";
        cout << "|Quadro|Processo|Frag|\n";
        cout << "----------------------\n";
        for (int p = 0; p < 8; ++p) {
            cout << "| [" << p+1 << "]  |"
            << " {P" << processo_alocado[p] << "}   "
            << "|" << memoria[p] << "|"
            << "\n|--------------------|" << endl;
        }


        // então é preciso saber seu tamanho,
        int tam_processo;
        cout << "Manda o tamanho do processo n° " << i + 1 << ", em bytes: ";
        cin >> tam_processo;

        // Esse true pode ser modificado
        processos.push_back(make_pair(true, tam_processo));

        // calcular quantas páginas ele vai tomar
        int paginas_necessarias = (tam_processo / PAGE_SIZE) + 1;
        // if (paginas_necessarias == 0) paginas_necessarias = 1;

        // e atribuí-lo às páginas
        for (int j = 0; j < paginas_necessarias; ++j) {
            bool pagina_livre_encontrada = false;
            
            bool alocou_tudo = false;
            for (int k = 0; k < 8; ++k) {
            
                if (!ocupada[k]) {
                    

                    if (tam_processo > PAGE_SIZE) {
                        tam_processo -= PAGE_SIZE;
                        memoria[k] = 0;
                    }
                    else {
                        memoria[k] -= tam_processo;
                        alocou_tudo = true;
                    }

                    ocupada[k] = true;
                    pagina_livre_encontrada = true;
                    processo_alocado[k] = i+1;
                    if (alocou_tudo) break;

                }
            }
            if (alocou_tudo) break;

            // Não tem como alocar o processo se não tem páginas suficientes pra ele
            if (!pagina_livre_encontrada) {
                cout << "Não deu pra alocar o processo n° " << i + 1 << " (falta de página).\n";

                // Indico que o processo não foi alocado.
                processos[i].first = false;

                // Desalocar o espaço gasto por ele até agora
                for (int n = 0; n < 8; ++n) {
                    if (processo_alocado[n] == i+1) {
                        memoria[n] = PAGE_SIZE;
                        processo_alocado[n] = 0;
                        ocupada[n] = false;
                    }
                }
                break;
            }
        }
    }


    int processo_escolhido{};
    cout << "Alocação concluida, escolha um processo: ";
    cin >> processo_escolhido;
    cout << "Agora escolha um endereço lógico (Em números decimais) : ";
    int endereço_logico{};
    cin >> endereço_logico;
    
    // O processo não está alocado
    if(!processos[processo_escolhido - 1].first){
        cout << "O processo não foi alocado. \n";
    }
    else if(processos[processo_escolhido - 1 ].second < endereço_logico or endereço_logico < 0){
        cout << "Endereço inválido. \n";
    }
    else{
        int memoria_total = 0;
        int pag_do_processo = 0;
        for(int x = 0; x < 8; x++){
            
            if(processo_alocado[x] == processo_escolhido){
                pag_do_processo++;
                memoria_total += PAGE_SIZE - memoria[x];
                if(memoria_total >= endereço_logico){
                    cout << "O endereço está na " << pag_do_processo << "° página do processo e no " << x+1 << "° quadro \n";
                    cout << "O deslocamento dentro da página foi de " << endereço_logico%PAGE_SIZE << '\n'; 
                    cout << "O endereço físico é: " << PAGE_SIZE*x + endereço_logico%PAGE_SIZE << '\n';
                    break;
                }
            } 
        }
    }


    // mostrar a tabela de paginação final
        cout << "--[TABELA DE PÁGINA]--\n";
        cout << "|Quadro|Processo|Frag|\n";
        cout << "----------------------\n";
        for (int p = 0; p < 8; ++p) {
            cout << "| [" << p+1 << "]  |"
            << " {P" << processo_alocado[p] << "}   "
            << "|" << memoria[p] << "|"
            << "\n|--------------------|\n";
        }
        cout << "A tabela de paginação ficou assim, ao final." << endl;

    return 0;
}