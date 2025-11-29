#pragma once

#include <string>
#include <fstream>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

class Atendimento {
public:
    Atendimento(unsigned int chefId, unsigned int mesaId);
    ~Atendimento();

    void enviarPedido(const std::string& pedido);
    void iniciar(); // rotina do processo filho (bloqueante)

private:
    unsigned int chefId;
    unsigned int mesaId;
    int fd[2];
    pid_t pid;
    std::string nomeArquivo;
    std::string quemSou;
    std::ofstream log;
};
