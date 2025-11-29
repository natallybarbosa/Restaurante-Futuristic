
#define RESTAURANTE_HPP

#include <vector>
#include <queue>
#include <string>
#include <memory>
#include "chef.hpp"

class Restaurante {
private:
    // lista de chefs (shared_ptr pq você já usa assim)
    std::vector<std::shared_ptr<Chef>> chefes;

    // fila O(1) de chefs livres (índices)
    std::queue<int> chefsLivres;

    // fila de espera: (mesa, fila de pedidos da mesa)
    std::queue<std::pair<int, std::queue<std::string>>> filaEspera;

    // mesa → id do chef que está atendendo (-1 = nenhum)
    std::vector<int> mesaParaChefe;//armazena qm está atendendo cada mesa


public:
    Restaurante(int nChefes, int nMesas);

    // processa pedidos normais + fim
    void processarPedido(int mesa, const std::string& pedido);

    // libera o chef quando mesa terminou
    void liberarChefe(int mesa);

    // fecha tudo
    void finalizar();
    //pro pedido acumular corretamente 
    void adicionarPedidoNaFila(int mesa, const std::string& pedido);
};



