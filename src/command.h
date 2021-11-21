#pragma once

#include "bypto/common/types.h"
#include "bypto/exchange.h"
#include "bypto/strategy.h"
#include "bypto/indicator/trend.h"

#include <map>

using namespace bypto::account;
using namespace bypto::exchange;
using namespace bypto::strategy;
using namespace bypto::indicator;

struct CommandFlags {
    std::string m_sym_flag;
    std::string m_exch_flag;
    std::string m_strat_flag;
    std::string m_ind_flag;
};

struct CommandOptions {
    Symbol m_sym;
    TagExchange m_exchange;
    TagStrategy m_strategy;
    TagIndicator m_indicator;
    CommandFlags m_cf;
};

std::map<std::string,TagExchange> map_exchange;
std::map<std::string,TagStrategy> map_strategy;
std::map<std::string,TagIndicator> map_indicator;

void populate_commands() {
    map_exchange.insert({"backtest",TagExchange::BackTest});
    map_exchange.insert({"binance",TagExchange::Binance});
    map_exchange.insert({"binance-test",TagExchange::BinanceTest});
    map_strategy.insert({"crossover",TagStrategy::Crossover});
    map_indicator.insert({"ema",TagIndicator::ExponentialMA});
    map_indicator.insert({"sma",TagIndicator::SimpleMA});
}

Error<CommandOptions> parse_commands(const CommandFlags &cf) {
    auto exch_it  = map_exchange.find(cf.m_exch_flag);
    auto strat_it = map_strategy.find(cf.m_strat_flag);
    auto ind_it   = map_indicator.find(cf.m_ind_flag);

    if (cf.m_exch_flag != "" && exch_it == map_exchange.end()) {
        return std::string("exchange "+cf.m_exch_flag+" is not supported");
    }
    if (cf.m_strat_flag != "" && strat_it == map_strategy.end()) {
        return std::string("strategy "+cf.m_strat_flag+" is not supported");
    }
    if (cf.m_ind_flag != "" && ind_it == map_indicator.end()) {
        return std::string("indicator "+cf.m_ind_flag+" is not supported");
    }
    
    auto exch  = cf.m_exch_flag  == "" ? TagExchange::BackTest  : exch_it->second;
    auto strat = cf.m_strat_flag == "" ? TagStrategy::Crossover : strat_it->second;
    auto ind   = cf.m_ind_flag   == "" ? TagIndicator::SimpleMA  : ind_it->second; 

    return CommandOptions {Symbol("BTC","USDT"),exch,strat,ind,cf};

}