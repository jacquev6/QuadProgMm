#include <iostream>
#include <QP/QP.hpp>

int main() {
{% for v in variables %}    QP::Variable {{ v }};
{% endfor %}
    QP::Objectives objectives;
{% for o in objectives %}    objectives.push_back( QP::Objective::{{ o }} );
{% endfor %}
    QP::Constraints constraints;
{% for c in constraints %}    constraints.push_back( {{ c|safe }} );
{% endfor %}
    QP::Problem p( objectives, constraints );

    std::cout << "Cost of the optimal solution: " << p.solve() << std::endl;
{% for v in variables %}    std::cout << "Value of {{ v }}: " << {{ v }} << std::endl;
{% endfor %}}

