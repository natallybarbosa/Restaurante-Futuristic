#pragma once
#include <string>
#include <fstream>

// forward  declaration para evitar include circular

class Atendimento;

class Chef {
public:
    Chef(unsigned int id, const std::string& nome);
    ~Chef();

    void iniciarAtendimento(unsigned int mesa);
    void prepararPedido(const std::string& pedido);
    void encerrarMesa();
    bool livre() const;

private:
    unsigned int id;
    std::string nome;
    unsigned int mesaAtual;
    Atendimento* atendimento = nullptr;

    std::string nomeArquivo;
    std::ofstream log;

    void registrarInicio(unsigned int mesa);
    void registrarPedido(const std::string& pedido);
    void registrarFim();
};

