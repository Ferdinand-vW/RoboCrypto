struct CommandFlags {
    std::string m_sym_flag;
    std::string m_exchange_flag;
    std::string m_strategy_flag;
    std::string m_indicator_flag;
};

struct CommandOptions {
    Symbol m_sym;
    TagExchange m_exchange;
    TagStrategy m_strategy;
    TagIndicator m_indicator;
    CommandFlags m_cf;
};