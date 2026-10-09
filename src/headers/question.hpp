/**
 * @file question.hpp
 * @brief Declaração da classe Question, utilizada para representar o texto de uma pergunta.
 */

#ifndef QUESTION_HPP
#define QUESTION_HPP

#include <string>

/**
 * @class Question
 * @brief Encapsula a representação textual de uma pergunta.
 */
class Question {
   private:
    std::string text_;  ///< O texto subjacente da pergunta.

   public:
    /**
     * @brief Constrói uma nova instância da classe Question.
     *
     * @param text O texto da pergunta que será armazenado.
     */
    Question(std::string& text);

    /**
     * @brief Obtém o texto desta pergunta.
     *
     * @return std::string O texto da pergunta.
     */
    [[nodiscard]] auto get_text() const -> std::string;
};

#endif
