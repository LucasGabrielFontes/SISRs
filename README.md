# Slack Induction by String Removals (SISRs)

Implementação do algoritmo heurístico Slack Induction by String Removals (SISRs) para resolver o Capacitated Vehicle Routing Problem (CVRP).

O CVRP é um problema clássico de otimização combinatória, no qual busca-se minimizar o custo (distância) total de rotas de veículos capacitados (limitados por sua capacidade máxima suportada) que atendem a um conjunto de clientes com demandas específicas, respeitando restrições de capacidade e rota.

Execução: ./cvrp ./instances/nomedainstancia.cvrp

A função readData() lê o arquivo de instância e armazena a matriz de distâncias e o número de clientes nas variáveis matrizAdj e dimension, respectivamente.
