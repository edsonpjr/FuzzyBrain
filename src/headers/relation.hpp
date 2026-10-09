/**
 * @file relation.hpp
 * @brief Declaração da classe Relation, que armazena um valor de grau de pertinência.
 */

#ifndef RELATION_HPP
#define RELATION_HPP

/**
 * @class Relation
 * @brief Armazena um grau de pertinência numérico utilizado em relações difusas (fuzzy).
 */
class Relation {
   private:
    double membership_degree_;  ///< Grau de pertinência numérico da relação.
    
   public:
    /**
     * @brief Constrói uma nova instância de Relation com um grau de pertinência inicial.
     *
     * @param membership_degree O valor do grau de pertinência (geralmente entre 0.0 e 1.0).
     */
    explicit Relation(const double& membership_degree);

    /**
     * @brief Obtém o grau de pertinência armazenado.
     *
     * @return double O valor do grau de pertinência.
     */
    [[nodiscard]] auto get_membership_degree() const -> double;
};

#endif
