#include <iostream>
#include <vector>
#include <stack>
#include <string>

/*
    0 - estado de partida 
    1 - estado de chegada 
    2 - simbolo de entrada 
    3 - simbolo topo da pilha
    4 - string a ser escrita na pilha
*/
struct Transicao {
    int partida {};
    int destino {};
    char entrada {};
    char topo_pilha {};
    std::string escrita {};
};

class Automato {
private:
    int qtd_estados {};                         // quantidade de estados do automato
    int qtd_simbolos_alfabeto_entrada {};       // quantidade de simbolos do alfabeto de entrada
    std::vector<char> alfabeto_entrada;         // alfabeto de entrada
    int qtd_simbolos_alfa_pilha {};             // quantidade de simbolos do alfabeto de entrada
    std::vector<char> alfabeto_pilha;           // alfabeto de pilha
    int estado_inicial {};                      // estado inicial
    char simbolo_inicial_pilha {};              // simbolo inicial pilha
    int qtd_estados_finais {};                  // quantidade de estados finais
    std::vector<int> estados_finais;            // estados finais
    int qtd_transicoes;                         // quantidade de transicoes
    std::vector<Transicao> transicoes;          // quantidade de transições
    std::string entrada {""};                   // entrada do automato
    std::stack<char> pilha;                     // pilha do automato

    bool aceitacao = false;                     // aceitação do automato
    std::string saida {""};                     // saida do automato
public:
    Automato() {
        std::cin >> this->qtd_estados;

        std::cin >> this->qtd_simbolos_alfabeto_entrada;

        for (int i = 0; i < this->qtd_simbolos_alfabeto_entrada; i++) {
            char simbolo;
            std::cin >> simbolo;
            this->alfabeto_entrada.push_back(simbolo);
        }

        std::cin >> this->qtd_simbolos_alfa_pilha;

        for (int i = 0; i < this->qtd_simbolos_alfa_pilha; i++) {
            char simbolo;
            std::cin >> simbolo;
            this->alfabeto_pilha.push_back(simbolo);
        }

        std::cin >> this->estado_inicial;

        std::cin >> this->simbolo_inicial_pilha;

        std::cin >> this->qtd_estados_finais;

        for (int i = 0; i < this->qtd_estados_finais; i++) {
            int estado;
            std::cin >> estado;
            this->estados_finais.push_back(estado);
        }

        std::cin >> this->qtd_transicoes;

        for (int i = 0; i < this->qtd_transicoes; i++) {
            Transicao transicao;
            std::cin >> transicao.partida;
            std::cin >> transicao.destino;
            std::cin >> transicao.entrada;
            std::cin >> transicao.topo_pilha;
            std::cin >> transicao.escrita;
            this->transicoes.push_back(transicao);
        }

        std::cin >> this->entrada;

        this->pilha.push(this->simbolo_inicial_pilha);
    }

    void iniciar() {
        this->transicao(this->estado_inicial, this->entrada, this->pilha);
    }

    std::string print_pilha(std::stack<char> pilha) {
        std::string pilha_str = "";
        std::stack<char> pilha_aux = pilha;
        while (!pilha.empty()) {
            pilha_str += pilha.top();
            pilha.pop();
        }
        return pilha_str;
    }

    void verify_aceitacao(int estado, std::string entrada, std::stack<char> pilha) { // OK
        for (int e : this->estados_finais) {
            if (e == estado && entrada == "e" && pilha.top() == this->simbolo_inicial_pilha) {
                this->aceitacao = true;
            }
        }
    }
    
    void transicao(int estado, std::string entrada, std::stack<char> pilha) {
        if (entrada == "") {
            entrada = "e";
        }

        this->saida += "; (" + std::to_string(estado) + ", " + entrada + ", " + this->print_pilha(pilha);

        for (Transicao t : this->transicoes) {
            if ((t.partida == estado) && (t.entrada == entrada[0] || t.entrada == 'e') && (t.topo_pilha == pilha.top())) {

                // cout << t.partida << " " << t.destino << " " << t.entrada << " " << t.topo_pilha << " " << t.escrita << endl;

                std::stack<char> pilha_aux = pilha;
                std::string aux = entrada;

                if (t.escrita.size() == 2) { 
                    pilha_aux.push(t.escrita[0]);
                    this->transicao(t.destino, aux.substr(1), pilha_aux);
                } else if (t.escrita == "e" && t.entrada == 'e') {
                    pilha_aux.pop();
                    this->transicao(t.destino, aux, pilha_aux);
                } else if (t.escrita == "e") {
                    pilha_aux.pop();
                    if (pilha_aux.empty()) {
                        pilha_aux.push('e');
                    }
                    this->transicao(t.destino, aux.substr(1), pilha_aux);
                } else if (t.entrada == 'e') {
                    this->transicao(t.destino, aux, pilha_aux);
                } else {
                    this->transicao(t.destino, aux.substr(1), pilha_aux);
                }
            }
        }

        this->verify_aceitacao(estado, entrada, pilha);

        this->saida += ")";
    }

    friend std::ostream& operator<<(std::ostream& os, const Automato& aut){
        os << aut.saida.substr(2) << "\n";

        if (aut.aceitacao) {
            os << "aceita" << "\n";
        } else {
            os << "rejeita" << "\n";
        }

        return os;
    }
};

int main(){
    Automato aut;
    aut.iniciar();
    std::cout << aut;

    return 0;
}