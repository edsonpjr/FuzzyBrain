/**
 * @file dataset_reader.hpp
 * @brief Declarações para leitura do conjunto de dados (dataset) utilizado pela aplicação.
 */

#ifndef DATASET_READER_HPP
#define DATASET_READER_HPP

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "candidate.hpp"
#include "question.hpp"
#include "relation.hpp"

/**
 * @struct CandidateRelation
 * @brief Associa um Candidato (Candidate) a uma Pergunta (Question) através de uma Relação
 * (Relation/grau de pertinência).
 */
struct CandidateRelation {
    Candidate candidate; ///< A entidade do candidato.
    Relation relation{0}; ///< A relação (grau de pertinência) associada ao candidato.
};

/**
 * @struct QuestionRow
 * @brief Representa uma pergunta e suas respectivas relações com todos os candidatos.
 */
struct QuestionRow {
    Question question; ///< A pergunta.
    std::vector<CandidateRelation> candidate_relations; ///< Relações entre a pergunta e cada candidato.
};

/**
 * @class DatasetReader
 * @brief Responsável por carregar e analisar (parse) o arquivo do conjunto de dados para estruturas
 * em memória.
 */
class DatasetReader {
  public:
    /**
     * @brief Construtor padrão para DatasetReader.
     */
    DatasetReader() = default;

    /**
     * @brief Carrega um conjunto de dados a partir de um arquivo.
     *
     * @param file_path Caminho para o arquivo do conjunto de dados. Se for relativo, será resolvido
     * a partir do caminho de recursos (resources).
     * @return std::vector<QuestionRow> Matriz analisada contendo as perguntas e as relações dos
     * candidatos.
     * @throws std::exception Se a análise falhar ou se houver erros irrecuperáveis de E/S de
     * arquivo.
     */
    [[nodiscard]] auto load_file(const std::filesystem::path& file_path)
        -> std::vector<QuestionRow>;

  private:
    inline static const std::string_view resources_path = "resources"; ///< Diretório padrão de recursos utilizado para caminhos relativos.

    /**
     * @brief Analisa uma única linha contendo valores de associação (pertinência).
     *
     * @param line A linha de entrada contendo valores numéricos separados por espaços em branco.
     * @param expected_count Quantidade esperada de valores (candidatos); utilizada para validação.
     * @return std::vector<double> Valores numéricos de associação analisados.
     * @throws std::exception Se a análise falhar ou se a quantidade de valores não corresponder a
     * expected_count.
     */
    static auto parse_association_line(const std::string& line, size_t expected_count) -> std::vector<double>;

    /**
     * @brief Lê uma única linha de um ifstream, removendo o caractere de quebra de linha CR (\r),
     * se presente.
     *
     * @param file O fluxo de entrada do arquivo (file stream) para leitura.
     * @param out String de destino que receberá o conteúdo da linha.
     * @return true Se a linha foi lida com sucesso.
     * @return false Se atingir o fim do arquivo (EOF).
     */
    static auto read_line(std::ifstream& file, std::string& out) -> bool;

    /**
     * @brief Ajusta o caminho do arquivo para um caminho absoluto, considerando o diretório de
     * recursos (resources) se necessário.
     *
     * @param file_path Caminho do arquivo a ser ajustado. Se for relativo, será resolvido a partir
     * do diretório de recursos.
     */
    static auto set_file_full_path(std::filesystem::path& file_path) -> void;

    /**
     * @brief Obtém um fluxo de entrada (ifstream) para o arquivo especificado.
     *
     * @param file_path Caminho do arquivo a ser aberto
     * @return std::ifstream Fluxo de entrada para leitura do arquivo.
     * @throws std::exception Se o arquivo não puder ser aberto ou se ocorrerem erros de E/S.
     */
    static auto get_file_stream(std::filesystem::path& file_path) -> std::ifstream;

    std::vector<Question> questions_; ///< Armazenamento temporário para as perguntas analisadas.
    std::vector<Candidate> candidates_; ///< Armazenamento temporário para os candidatos analisados.
};

#endif
