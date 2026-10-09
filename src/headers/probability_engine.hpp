/**
 * @file probability_engine.hpp
 * @brief Declaração da classe ProbabilityEngine, responsável pela distribuição de probabilidade
 * das hipóteses e pela seleção de perguntas por variância ponderada.
 */

#ifndef PROBABILITY_ENGINE_HPP
#define PROBABILITY_ENGINE_HPP

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <optional>
#include "candidate.hpp"
#include "dataset_reader.hpp"

/**
 * @class ProbabilityEngine
 * @brief Mantém a distribuição de probabilidade das hipóteses e escolhe a próxima pergunta a
 * partir do ganho de informação (variância ponderada).
 */
class ProbabilityEngine {
  public:
    /**
     * @brief Constrói uma nova instância de ProbabilityEngine (estado vazio).
     */
    ProbabilityEngine() = default;

    /**
     * @struct Hypothesis
     * @brief Representa a hipótese de que um determinado candidato é a resposta correta.
     */
    struct Hypothesis {
        std::unique_ptr<Candidate> candidate;  ///< Candidato associado (ponteiro inteligente).
        double probability = 0.0;              ///< Probabilidade atual da hipótese.
        int consecutive_zero_rounds = 0;       ///< Rodadas seguidas abaixo do limiar (epsilon).
    };
    
    /**
     * @brief Estrutura para encapsular os parâmetros de atualização de probabilidades.
     */
    struct ProbabilityParameters {
        size_t question_index;  ///< Índice da pergunta respondida (linha da matriz).
        double user_answer;     ///< Resposta do usuário, em [0.0, 1.0].
    };

    /**
     * @brief Estrutura retornada por get_strong_candidates() contendo os candidatos fortes.
     *
     * - indexed_hypotheses: Hipóteses dos candidatos considerados outliers (probabilidade acima do limiar) associadas a seus índices.
     * - mean: média das probabilidades.
     * - threshold: limiar usado para considerar um candidato como outlier.
     * - rounds_with_same_candidates: contador de rodadas com os mesmos candidatos fortes.
     */
    struct StrongCandidates {
        std::vector<std::pair<size_t, const Hypothesis*>>
            indexed_hypotheses;
        double mean = 0.0;
        double threshold = 0.0;
        int rounds_with_same_candidates = 0;
    };

    /**
     * @brief Inicializa as hipóteses com distribuição uniforme a partir da matriz carregada.
     *
     * Descarta qualquer estado anterior e cria uma hipótese para cada candidato, todas com a
     * mesma probabilidade. A matriz é apenas referenciada (não é copiada) e precisa permanecer
     * válida enquanto esta instância for utilizada.
     *
     * @param matrix Matriz de perguntas x candidatos carregada pelo DatasetReader.
     * @param debug_mode Indica se o modo debug está ativo.
     */
    void initialize(const std::vector<QuestionRow>& matrix, bool debug_mode);

    /**
     * @brief Retorna o índice da próxima pergunta a ser feita, ignorando as perguntas já registradas
     * por register_question_asked().
     *
     * @return Índice da pergunta de maior variância ponderada entre as ainda não feitas, ou nulo se não houver perguntas.
     */
    [[nodiscard]] auto get_next_question_index() const -> std::optional<std::size_t>;

    /**
     * @brief Atualiza as probabilidades das hipóteses a partir da resposta do usuário.
     *
     * Para cada hipótese, a similaridade é `1 - |pertinência teórica - resposta|` e a nova
     * probabilidade é a anterior multiplicada pela similaridade. Se o resultado ficar menor ou
     * igual a epsilon, a hipótese entra em "sobrevida" (probabilidade fixada em epsilon) e tem
     * consecutive_zero_rounds incrementado; ao atingir max_consecutive_zero_rounds rodadas
     * seguidas nessa condição, é eliminada definitivamente (probabilidade 0). Qualquer resultado
     * acima de epsilon zera esse contador. Ao final, as probabilidades são normalizadas para
     * somarem 1. Se todas as hipóteses forem eliminadas na mesma atualização, a distribuição
     * volta a ser uniforme (fallback).
     *
     * Comportamentos esperados (não são erros):
     * - Uma hipótese eliminada permanece em 0 para sempre (0 * similaridade = 0), exceto se o
     *   fallback uniforme for acionado.
     * - Uma hipótese em sobrevida tende a permanecer travada em epsilon por algumas rodadas,
     *   pois epsilon * similaridade normalmente continua menor ou igual a epsilon.
     *
     * Não faz nada se a instância não tiver sido inicializada. A resposta não é validada aqui;
     * quem chama deve garantir que `user_answer` esteja em [0.0, 1.0].
     *
     * @param question_index Índice da pergunta respondida (linha da matriz).
     * @param user_answer Resposta do usuário, em [0.0, 1.0].
     * @throws std::out_of_range Se question_index não for um índice válido da matriz.
     */
    void update_probabilities(const ProbabilityParameters& params);

    /**
     * @brief Registra uma pergunta como já feita, para que não seja escolhida novamente.
     *
     * Registrar o mesmo índice mais de uma vez não tem efeito adicional. Quem controla o fluxo
     * de perguntas decide quando chamar este método.
     *
     * @param question_index Índice da pergunta (linha da matriz) que já foi feita.
     */
    void register_question_asked(size_t question_index);

    /**
     * @brief Atualiza os candidatos fortes ("outliers") na distribuição atual se houver.
     *
     * Calcula a média e o desvio padrão populacional das probabilidades de todas as hipóteses.
     * Uma hipótese é considerada um candidato forte quando sua probabilidade excede
     * `média + peso * desvio padrão`, ou seja, quando ela se destaca
     * estatisticamente das demais. Hipóteses já eliminadas (probabilidade 0) entram no cálculo
     * normalmente e ajudam a evidenciar quem se destaca.
     *
     * Atualiza a estrutura strong_candidates_ com os candidatos fortes, a média das probabilidades e o limiar usado para
     * a classificação.
     */
     void update_strong_candidates();

    /**
     * @brief Informa se o motor de probabilidade já identificou um candidato para sugestão.
     *
     * Delega para get_strong_candidates() a identificação de candidatos fortes
     * (outliers) e avalia se há um candidato com probabilidade suficientemente alta e destacada
     * para ser sugerido:
     *  - se apenas um candidato forte: probabilidade acima de 70%;
     *  - se mais de um candidatos fortes: ter mais de 70% de probabilidade e diferença de
     * probabilidade em relação ao segundo candidato mais provável acima de 40%;
     *
     * @return true se motor identificou um candidato forte suficiente para ser sugerido; false caso
     * contrário.
     */
    [[nodiscard]] auto has_candidate_suggestion() const -> bool;

    /**
     * @brief Retorna o mapa de hipóteses.
     *
     * @return Referência constante para o mapa de hipóteses.
     */
    [[nodiscard]] auto get_hypotheses() const -> const std::unordered_map<size_t, Hypothesis>&;

    /**
     * @brief Retorna o texto do candidato com maior probabilidade atual.
     *
     * @return Texto do candidato mais provável, ou string vazia se não houver hipóteses.
     */
    [[nodiscard]] auto get_top_candidate_text() const -> std::string;

    /**
     * @brief Elimina o candidato com maior probabilidade (rejeição pelo usuário) e
     * renormaliza as demais probabilidades.
     *
     * Caso todas as hipóteses restantes sejam eliminadas, aciona o fallback para
     * distribuição uniforme.
     */
    void eliminate_top_candidate();

  private:
    static constexpr double variance_weight = 10.0;  ///< Peso aplicado no cálculo da variância.
    static constexpr double epsilon = 1e-6;          ///< Limiar mínimo seguro ("sobrevida").
    static constexpr int max_consecutive_zero_rounds = 3; ///< Rodadas até a eliminação definitiva.
    static constexpr int max_rounds_with_same_candidates = 3; ///< Número máximo de rodadas com os mesmos candidatos fortes seguencialmente sugeridos.
    static constexpr double outlier_std_dev_weight = 3.0; ///< Peso aplicado no desvio padrão para verificar candidatos descatacados (candidatos fortes).
    static constexpr double minimum_probability_suggestion = 0.7; ///< Probabilidade mínima para sugestão.
    static constexpr double minimum_probability_difference = 0.4; ///< Diferença mínima de probabilidade para sugestão.
    static constexpr size_t max_questions_drawn = 10;  ///< Número máximo de perguntas a serem consideradas para sorteio de pergunta inicial.

    /**
     * @brief Redefine todas as hipóteses para a distribuição uniforme.
     *
     * Atribui probabilidade `1 / total de hipóteses` a cada uma e zera consecutive_zero_rounds.
     * Não faz nada se não houver hipóteses.
     */
    void reset_to_uniform_distribution();

    /**
     * @brief Seleciona a pergunta de maior variância ponderada entre as ainda não feitas.
     *
     * Para cada pergunta, calcula a média dos graus de pertinência de todos os candidatos e,
     * em seguida, a variância ponderada pelas probabilidades atuais das hipóteses:
     * soma((pertinência - média)² * probabilidade * variance_weight) / total de candidatos.
     * Perguntas já registradas em asked_questions_ ou sem candidatos são ignoradas.
     *
    * Observação: quando há várias perguntas com alta variância, a implementação pode
    * escolher aleatoriamente uma entre as top N (N = max_question_sort) perguntas com
    * maior variância apenas na seleção da primeira pergunta; chamadas subsequentes
    * seguem a mesma heurística, mas sem esse sorteio inicial.
     *
     * @return Índice da pergunta de maior variância ponderada (ou uma escolha aleatória entre
     * as top N), ou nulo se não houver nenhuma pergunta disponível.
     */
    [[nodiscard]] auto select_question_by_weighted_variance() const -> std::optional<std::size_t>;

    /**
     * @brief Seleciona a próxima pergunta utilizando a candidatos fortes (heurística de outliers).
     *
     * A heurística considera apenas os candidatos fortes (outliers) atualmente
     * identificados em strong_candidates_. Para cada pergunta ainda não feita,
     * computa uma variância ponderada das pertinências teóricas dos candidatos
     * fortes; a variância é ponderada pela probabilidade da hipótese e por um
     * fator extra para o candidato mais forte. A pergunta escolhida é aquela
     * que maximiza essa variância ponderada — isto é, a que provavelmente
     * oferece maior ganho de informação sobre os candidatos fortes.
     * Perguntas já registradas em asked_questions_ ou sem candidatos são ignoradas.
     *
     * Comportamento e pré-condições:
     * - É esperado que a instância tenha sido inicializada (initialize()) antes
     *   do uso, e que strong_candidates_ já tenha sido atualizado.
     *
     * @return Índice da pergunta selecionada (std::optional<std::size_t>), ou
     *         std::nullopt se nenhuma pergunta puder ser selecionada.
     */
    [[nodiscard]] auto select_question_by_outliers() const -> std::optional<std::size_t>;

    const std::vector<QuestionRow>* matrix_ = nullptr;   ///< Matriz do Engine (não proprietário).
    std::unordered_map<size_t, Hypothesis> hypotheses_;  ///< Hipóteses por índice de candidato.
    StrongCandidates strong_candidates_;                 ///< Candidatos fortes identificados na última avaliação.
    std::unordered_set<size_t> asked_questions_;         ///< Índices das perguntas já feitas.
    bool debug_mode_ = false;                            ///< Indica se o modo debug está ativo.
};

#endif
