/**
 * @file candidate.hpp
 * @brief Declaração da classe Candidate, que representa um item de candidato textual.
 */

#ifndef CANDIDATE_HPP
#define CANDIDATE_HPP

#include <string>

/**
 * @class Candidate
 * @brief Representa um candidato com um texto associado.
 */
class Candidate {
   private: std::string text_; ///< O texto subjacente do candidato.

   public:
    /**
     * @brief Constrói uma nova instância da classe Candidate.
     *
     * @param text O conteúdo textual para o candidato.
     */
    Candidate(std::string text);

    /**
     * @brief Recupera o texto do candidato.
     *
     * @return std::string O texto armazenado para o candidato.
     */
    [[nodiscard]] auto get_text() const -> std::string;
};

#endif
