#pragma once

#include "bypto/common/types.h"
#include "bypto/exchange.h"
#include "bypto/indicator/trend.h"
#include "bypto/strategy.h"
#include "output.h"

#include <map>
#include <ranges>
#include <string>

using namespace bypto::account;
using namespace bypto::exchange;
using namespace bypto::strategy;
using namespace bypto::indicator;

struct CommandFlags {
    std::string m_sym_flag;
    std::string m_exch_flag;
    std::string m_strat_flag;
    std::string m_ind_flag;
    std::string m_write_flag;
    std::string m_tick_rate_flag;
    std::string m_max_run_time_flag;
};

struct CommandOptions {
    Symbol m_sym;
    TagExchange m_exchange;
    TagStrategy m_strategy;
    TagIndicator m_indicator;
    int m_tick_rate_s;
    int m_max_run_time_s;
    std::vector<OutputType> m_writers;
    CommandFlags m_cf;
};

std::map<std::string, TagExchange> map_exchange;
std::map<std::string, TagStrategy> map_strategy;
std::map<std::string, TagIndicator> map_indicator;

void populate_commands() {
    map_exchange.insert({"backtest", TagExchange::BackTest});
    map_exchange.insert({"binance", TagExchange::Binance});
    map_exchange.insert({"binance-test", TagExchange::BinanceTest});
    map_strategy.insert({"crossover", TagStrategy::Crossover});
    map_indicator.insert({"ema", TagIndicator::ExponentialMA});
    map_indicator.insert({"sma", TagIndicator::SimpleMA});
}

Error<CommandOptions> parse_commands(const CommandFlags &cf) {
    auto exch_it = map_exchange.find(cf.m_exch_flag);
    auto strat_it = map_strategy.find(cf.m_strat_flag);
    auto ind_it = map_indicator.find(cf.m_ind_flag);

    if (cf.m_exch_flag != "" && exch_it == map_exchange.end()) {
        return std::string("exchange " + cf.m_exch_flag + " is not supported");
    }
    if (cf.m_strat_flag != "" && strat_it == map_strategy.end()) {
        return std::string("strategy " + cf.m_strat_flag + " is not supported");
    }
    if (cf.m_ind_flag != "" && ind_it == map_indicator.end()) {
        return std::string("indicator " + cf.m_ind_flag + " is not supported");
    }

    auto exch = cf.m_exch_flag == "" ? TagExchange::BackTest : exch_it->second;
    auto strat = cf.m_strat_flag == "" ? TagStrategy::Crossover : strat_it->second;
    auto ind = cf.m_ind_flag == "" ? TagIndicator::SimpleMA : ind_it->second;

    auto tr = cf.m_tick_rate_flag;
    int tick_rate;
    auto [tr_ptr, ec] = std::from_chars(tr.data(), tr.data() + tr.size(), tick_rate);
    if (ec != std::errc()) {
        tick_rate = 60 * 15; // 15minutes
        std::cout << "Defaulting to tick rate of 15 minutes" << std::endl;
    }

    auto rt = cf.m_max_run_time_flag;
    int run_time;
    auto [rt_ptr, ec2] = std::from_chars(rt.data(), rt.data() + rt.size(), run_time);
    if (ec2 != std::errc()) {
        run_time = 60 * 60 * 4; // 4 hours
        std::cout << "Defaulting max running time to 4 hours" << std::endl;
    }

    char delim(',');
    std::vector<OutputType> outs;
    for (auto &word : utils::split(cf.m_write_flag, delim)) {
        if (word == "funds") {
            outs.push_back(OutputType::Fund);
        }
        if (word == "indicators") {
            outs.push_back(OutputType::Indicator);
        }
        if (word == "pnls") {
            outs.push_back(OutputType::PNL);
        }
        if (word == "orders") {
            outs.push_back(OutputType::Order);
        }
        if (word == "prices") {
            outs.push_back(OutputType::Price);
        }
    }

    return CommandOptions{Symbol("BTC", "USDT"), exch, strat, ind, tick_rate, run_time, outs, cf};
}