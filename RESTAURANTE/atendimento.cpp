#include "Atendimento.hpp"

#include <unistd.h>     // fork, pipe, read, write, close
#include <signal.h>     // kill, SIGKILL
#include <sys/types.h>  // pid_t
#include <sys/wait.h>
#include <iostream>
#include <cstdlib>
#include <cstring>      // strerror, etc

Atendimento::Atendimento(unsigned int chefId, unsigned int mesaId)//construtor da classe que recebe qual chef e qual mesa 
    : chefId(chefId), mesaId(mesaId)
{
    nomeArquivo = "Mesa_" + std::to_string(mesaId) + ".txt";//cria o nome do arquivo de log de cada mesa

    if (pipe(fd) < 0) {//fd é um array de dois inteiros um pra leitura, outro pra escrita
        perror("pipe");
        exit(EXIT_FAILURE);//caso de erro na criação de comunicação, cai fora
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // filho: ler do fd[0]
        quemSou = "Filho";
        close(fd[1]);
        iniciar();
        _exit(0);
    } else {
        // pai: escrever em fd[1]
        quemSou = "Pai";
        close(fd[0]);

        signal(SIGPIPE, SIG_IGN);
    }
}

Atendimento::~Atendimento() {//pai e filho se separam apos o fork 
    if (quemSou == "Pai") {
        // fechar escrita e matar o filho
        close(fd[1]);
        // tenta primeiro terminar graciosamente
        //kill(pid, SIGKILL);
        waitpid(pid, nullptr, 0);// evita filho zumbi
    } else {
        // filho já fechou no final da rotina
        close(fd[0]);
    }
}

void Atendimento::enviarPedido(const std::string& pedido) {
    // escreve incluindo o '\0' para o leitor poder usar strlen se quiser
    ssize_t nw = write(fd[1], pedido.c_str(), pedido.size() + 1);
    // Se o filho já saiu/fechou o lado de leitura, write falhará (EPIPE).
    // Como ignoramos SIGPIPE no pai, aqui podemos tratar o erro sem morrer.
    if (nw == -1) {
        if (errno == EPIPE) {
            // Filho fechou o pipe — não dá pra enviar mais pedidos.
            // Apenas logamos ou ignoramos silenciosamente (escolha do projeto).
            std::cerr << "Atendimento::enviarPedido: pipe fechado (EPIPE). mesa="
                      << mesaId << " chefId=" << chefId << "\n";
        } else {
            std::cerr << "Atendimento::enviarPedido erro write(): "
                      << std::strerror(errno) << "\n";
        }
    }
    (void)nw; // // ignorar retorno aqui; em app real trate erros
}


void Atendimento::iniciar() {
    log.open(nomeArquivo, std::ios::out | std::ios::app);
    if (!log.is_open()) {
        perror("abrir log mesa");
        return;
    }

    char buffer[256];

    while (true) {
        ssize_t n = read(fd[0], buffer, sizeof(buffer) - 1);
        if (n <= 0) break;
        buffer[n] = '\0';
        std::string msg(buffer);

        if (msg == "fim") {
            log << "\n--- Mesa finalizada ---\n";
            break;
        }

        log << " - " << msg << "\n";
        log.flush();
    }

    log.close();
    close(fd[0]);
}
