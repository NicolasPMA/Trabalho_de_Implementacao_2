#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <limits>

class ProdutoEstoque {
public:
    std::string nome;
    double preco;
    int quantidade;

    ProdutoEstoque() : nome(""), preco(0.0), quantidade(0) {}

    ProdutoEstoque(std::string nome, double preco, int quantidade)
        : nome(nome), preco(preco), quantidade(quantidade) {}
};

class EstoqueProdutos {
public:
    std::map<std::string, ProdutoEstoque> produtos;

    void adicionaProduto(const ProdutoEstoque& produto) {
        produtos[produto.nome] = produto;
    }

    bool atualizaEstoque(const std::string& nome, int quantidade) {
        if (produtos.find(nome) != produtos.end() && produtos[nome].quantidade >= quantidade) {
            produtos[nome].quantidade -= quantidade;
            return true;
        }
        return false;
    }

    void exibeProdutos() const {
        std::cout << "===================================================";
        std::cout << "\nProdutos disponiveis no estoque:\n";
        for (const auto& produto : produtos) {
            std::cout << "Produto: " << produto.second.nome
                      << ", Preco: " << produto.second.preco
                      << ", Quantidade: " << produto.second.quantidade << std::endl;
        }
    }
};

class Item {
public:
    std::string nome;
    int quantidade;

    Item(std::string nome, int quantidade)
        : nome(nome), quantidade(quantidade) {}
};

class CarrinhoCompra {
private:
    EstoqueProdutos& estoque;
    std::vector<Item> itens;

public:
    CarrinhoCompra(EstoqueProdutos& estoque)
        : estoque(estoque) {}

    void adicionaItem(const Item& item) {
        if (estoque.atualizaEstoque(item.nome, item.quantidade)) {
            itens.push_back(item);
        } else {
            std::cerr << "Quantidade indisponivel no estoque para o item: " << item.nome << std::endl;
        }
    }

    double calculaTotal() const {
        double total = 0;
        for (const auto& item : itens) {
            if (estoque.produtos.find(item.nome) != estoque.produtos.end()) {
                total += estoque.produtos.at(item.nome).preco * item.quantidade;
            }
        }
        return total;
    }

    void exibeCarrinho() const {
        std::cout << "\nItens no carrinho:\n";
        for (const auto& item : itens) {
            std::cout << "Item: " << item.nome << ", Quantidade: " << item.quantidade << std::endl;
        }
        std::cout << "Total: " << calculaTotal() << std::endl;
    }
};

int main() {
    EstoqueProdutos estoque;
    estoque.adicionaProduto(ProdutoEstoque("monitor", 500, 100));
    estoque.adicionaProduto(ProdutoEstoque("telefone", 150, 300));
    estoque.adicionaProduto(ProdutoEstoque("teclado", 70, 50));
    estoque.adicionaProduto(ProdutoEstoque("mouse", 50, 50));

    CarrinhoCompra carrinho(estoque);
    int opcao;

    do {
        estoque.exibeProdutos();
        std::cout << "\nMenu:\n";
        std::cout << "1. Adicionar item ao carrinho\n";
        std::cout << "2. Ver total do carrinho\n";
        std::cout << "3. Finalizar compra e sair\n";
        std::cout << "Escolha uma opcao: ";
        std::cin >> opcao;

        // Verificaçao de erro de entrada
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Entrada invalida. Por favor, tente novamente.\n";
            continue;
        }

        switch (opcao) {
        case 1: {
            std::string nome;
            int quantidade;
            std::cout << "Nome do item: ";
            std::cin >> nome;
            std::cout << "Quantidade do item: ";
            std::cin >> quantidade;

            // Verificação de erro de entrada
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Entrada invalida. Por favor, tente novamente.\n";
                continue;
            }

            carrinho.adicionaItem(Item(nome, quantidade));
            break;
        }
        case 2: {
            carrinho.exibeCarrinho();
            break;
        }
        case 3: {
            std::cout << "Compra finalizada!\n\n";
            std::cout << "Estoque apos a compra:\n";
            for (const auto& produto : estoque.produtos) {
                std::cout << "Produto: " << produto.second.nome << ", Quantidade: " << produto.second.quantidade << std::endl;
            }
            std::cout << "Saindo...\n";
            break;
        }
        default:
            std::cerr << "Opcao invalida! Tente novamente.\n";
            break;
        }
    } while (opcao != 3);

    return 0;
}
