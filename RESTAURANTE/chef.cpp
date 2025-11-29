#include "chef.hpp"

#include "atendimento.hpp"
#include <iostream> //PRA USAR O std::cout
#include <csignal>
#include <unistd.h>

//CONSTRUTOR DO CHEF
Chef::Chef(unsigned int id, const std::string& nome)
    : id(id), nome(nome), mesaAtual(0), atendimento(nullptr)
{
    nomeArquivo = "Chefe_" + nome + ".txt";
    log.open(nomeArquivo, std::ios::out);
    if (log.is_open()) {
        log << "Chef " << nome << " (ID " << id << ")\n\n"; //pra deixar mais bonito
        log.close();
    }
}

Chef::~Chef() {
    if (atendimento != nullptr) {
        delete atendimento;
        atendimento = nullptr;
    }
}

bool Chef::livre() const {
    return atendimento == nullptr;
}

//Aqui vai inciar o atendimento de uma mesa
//basic: se o chef não ta ocupado, ele começa um novo atendimento
void Chef::iniciarAtendimento(unsigned int mesa) {
    if (!livre()) return;

    mesaAtual = mesa;
    registrarInicio(mesa);//salva no arquivo q começou a mesa

    atendimento = new Atendimento(id, mesa); //cria obj, e cria um atendimento para cada chef
//da um feedbaclk
    std::cout << ">> " << nome << " começou a preparar o pedido da mesa "<< mesa << "!" << std::endl;
}
// Chef recebe um pedido, registra no arquivo e manda pro atendimento tratar
void Chef::prepararPedido(const std::string& pedido) {
    if (!atendimento) return;

    // se o pedido for "fim" -> finaliza
    if (pedido == "fim") {
        encerrarMesa();
        return;
    }

    // pedido normal
    registrarPedido(pedido);
    atendimento->enviarPedido(pedido);

    std::cout << "Resposta do chef " << nome << ": O pedido está pronto --> para a mesa:" << mesaAtual << std::endl;
}


void Chef::encerrarMesa() {//só finalizará quando for digitado 'fim', não há time automatico
    if (!atendimento) {
        return;
    }
    registrarFim();
    atendimento->enviarPedido("fim");//sinaliza q terminouu

    delete atendimento;//libera memoria
    atendimento = nullptr;

    std::cout << ">> " << nome << " finalizou o atendimento da mesa ;)" << std::endl;
    mesaAtual = 0;
}

void Chef::registrarInicio(unsigned int mesa) {
    log.open(nomeArquivo, std::ios::app);//abre em modo append
    if (log.is_open()) {
        log << "Mesa " << mesa << ":\n"; //titulo da mesa
        log.close();
    }
}
//COLOCA O PEDIDO NO ARQUIVO
void Chef::registrarPedido(const std::string& pedido) {
    log.open(nomeArquivo, std::ios::app);
    if (log.is_open()) {
        log << " - " << pedido << "\n";//só pra ficar bonitnho
        log.close();
    }
}
//da um espaçamento entre mesas no arquivo
void Chef::registrarFim() {
    log.open(nomeArquivo, std::ios::app);
    if (log.is_open()) {
        log << "\n"; //pula linha pra separar
        log.close();
    }
}