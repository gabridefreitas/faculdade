# Sistemas de Gerencia de Banco de Dados

## Unisinos - 2023/2

## Atividade 8

### Integrantes do grupo

- Eduardo Bernardi
- Gabriel de Freitas Adolfo
- Víctor Hugo Haberkamp
- Vinicius Ferreira Marta Flores

### 1 - Ler artigo sobre SQL, NOSQL e NEWSQL

### 2 - Implementar uma collection no MongoDB e praticar comandos do material de notas de aula

#### Realizada implementação de exemplo para uma seção de comentários

- Nodejs com Express
- MongoDB sample: `sample_mflix`
  - Collection: `comments`

Para executar o projeto:

1. Execute o comando `yarn`
1. Execute o comando `yarn start`
1. Importe a [collection](./collection/SGBD%20-%20Atividade%208.postman_collection.json) no seu Postman
1. Realize as chamadas para os endpoints

### 3 - Escolher um BD NoSQL, preferencialmente o do TGB, para demonstrar operação SELECT em SQL x no BD/modelo escolhido

#### mySQL x Amazon Neptune

A linguagem SQL é usada para consultar o MySQL, enquanto o Amazon Neptune utiliza uma linguagem de consulta especializada chamada SPARQL para consultas em grafo.

Exemplo de consulta em MySQL:

```sql
SELECT Nome
FROM Pessoas
WHERE Idade > 30;
```

> Esta query retornaria uma lista de nomes de pessoas com mais de 30 anos da tabela "Pessoas".

Exemplo de consulta em Amazon Neptune:

```sparql
PREFIX : <http://namespace/> 

SELECT ?Nome
WHERE {
  ?pessoa :Nome ?Nome .
  ?pessoa :Idade ?Idade .
  FILTER (?Idade > 30)
}
```

> Esta query retornaria uma lista de nomes de pessoas com mais de 30 anos da tabela "Pessoas".
