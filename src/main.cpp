// RoboCrypto.cpp : Defines the entry point for the application.
//

#include "bypto/common/csv.h"
#include "bypto/common/either.h"
#include "bypto/common/types.h"
#include "bypto/common/utils.h"
#include "bypto/data/binance.h"
#include "bypto/data/price.h"
#include "bypto/exchange/backtest.h"
#include "bypto/exchange/runner.h"
#include "bypto/order/order.h"
#include "bypto/strategy.h"
#include "bypto/strategy/crossover.h"
#include "command.h"
#include "exchange.h"
#include "output.h"

#include <array>
#include <boost/asio/io_context.hpp>
#include <boost/program_options.hpp>
#include <boost/stacktrace.hpp>
#include <execinfo.h>

#include <binapi/api.hpp>
#include <binapi/enums.hpp>
#include <optional>
#include <tao/pq.hpp>
#include <tao/pq/connection.hpp>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

using namespace bypto::account;
using namespace bypto::exchange;
using namespace bypto::strategy;
using namespace bypto::indicator;
namespace po = boost::program_options;

void my_segfault_handler(int sig)
{
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

template <typename E, PriceSource P>
Error<bool> run_with_exchange(CommandOptions &opts, OutputWriter &ow, Runner<E, P> &runner)
{
    auto strat_tag = opts.m_strategy;
    auto ind_tag = opts.m_indicator;

    if (ind_tag == TagIndicator::ExponentialMA && strat_tag == TagStrategy::Crossover)
    {
        ExponentialMA ma;
        Crossover<ExponentialMA, P> strat(ma);
        auto res = runner.run(opts.m_sym, strat);
        ow.write_indicators(strat.get_indicator_data());
        return res;
    }
    else if (ind_tag == TagIndicator::SimpleMA && strat_tag == TagStrategy::Crossover)
    {
        SimpleMA sma;
        Crossover<SimpleMA, P> strat(sma);
        auto res = runner.run(opts.m_sym, strat);
        ow.write_indicators(strat.get_indicator_data());
        return res;
    }
    else
    {
        return std::string("could not match strategy/indicator pattern: " + opts.m_cf.m_strat_flag +
                           "/" + opts.m_cf.m_ind_flag);
    }
}

template <typename T, PriceSource P> void write_out_data(const Exchange<T, P> &e, OutputWriter &ow)
{
    ow.write_funds(e.get_funds_csv());
    ow.write_pnls(e.get_pnls_csv());
    ow.write_orders(e.get_orders_csv());
    ow.write_prices(e.get_prices_csv());
}

Error<bool> run(CommandOptions opts, OutputWriter &ow)
{
    auto exch_tag = opts.m_exchange;

    boost::asio::io_context io;
    if (exch_tag == TagExchange::BackTest)
    {
        auto bt = backtest(opts);
        Runner<BackTest, PriceSource::Kline> runner(bt);

        auto res = run_with_exchange(opts, ow, runner);

        write_out_data(bt, ow);
        return res;
    }

    else if (exch_tag == TagExchange::Binance)
    {
        auto api = connect_prod_network(io);
        auto bin = binance(opts, api);
        Runner<Binance, PriceSource::Spot> runner(bin);

        auto res = run_with_exchange(opts, ow, runner);

        write_out_data(bin, ow);
        return res;
    }

    else if (exch_tag == TagExchange::BinanceTest)
    {
        auto api = connect_test_network(io);
        auto bin = binance(opts, api);
        Runner<Binance, PriceSource::Spot> runner(bin);

        auto res = run_with_exchange(opts, ow, runner);

        write_out_data(bin, ow);
        return res;
    }

    else
    {
        return std::string("could not match exchange pattern: " + opts.m_cf.m_exch_flag);
    }
}

int main()
{
    // set up exceptions handlers
    signal(SIGSEGV, my_segfault_handler);

    bypto::common::types::Symbol sym("BTC", "USDT");

    CommandFlags cf = {"BTCUSDT", "backtest", "crossover", "ema"};

    populate_commands();
    auto e_opts = parse_commands(cf);
    if (e_opts.isLeft())
    {
        std::cout << "parse error: " << e_opts.left() << std::endl;
        return EXIT_FAILURE;
    }

    OutputWriter ow;
    ow.set(OutputType::Fund);
    ow.set(OutputType::Indicator);
    ow.set(OutputType::Order);
    ow.set(OutputType::PNL);
    ow.set(OutputType::Price);
    auto res = run(e_opts.right(), ow);
    if (res.isLeft())
    {
        std::cout << res.left() << std::endl;
    }

    // std::cout << res << std::endl;

    // auto pm = bte.get_price_map().right();
    // auto updated_account = bte.get_account_info().right();

    // std::cout << "Using price map for valuation: " << pm << std::endl;
    // auto in_val = initial_account.value("USDT", pm);
    // std::cout << "Initial account: " << initial_account << std::endl;
    // std::cout << "Value without strategy: " << in_val.right().ppValue() << std::endl;
    // auto upd_val = updated_account.value("USDT", pm);
    // std::cout << "Updated account: " << updated_account << std::endl;
    // std::cout << "Value with strategy: " << upd_val.right().ppValue() << std::endl;

    // std::ofstream csv("ma.csv");

    // auto csvHeader = collector.csv_header();
    // auto csvData = collector.csv_data();

    // csv::write(csvHeader, csvData, csv);

    // auto open_time = t
    // const auto pk = std::getenv("BINANCE_TEST_PUBLIC_KEY");
    // const auto sk = std::getenv("BINANCE_TEST_SECRET_KEY");

    // std::cout << "public key: " << pk << std::endl;
    // std::cout << "secret key: " << sk << std::endl;
    // boost::asio::io_context io;
    // binapi::rest::api api{
    //      io
    //     ,"testnet.binance.vision"
    //     ,"443"
    //     ,pk // can be empty for non USER_DATA reqs
    //     ,sk // can be empty for non USER_DATA reqs
    //     ,10000 // recvWindow
    // };

    // auto res = api.price("BTCUSDT");
    // if (!res) {
    //     std::cerr << "get price error: " << res.errmsg << std::endl;

    //     return EXIT_FAILURE;
    // }

    // std::cout << "price: " << res.v << std::endl;

    // auto account = api.account_info();
    // if ( !account ) {
    //     std::cerr << "account info error: " << account.errmsg << std::endl;
    //     return EXIT_FAILURE;
    // }

    // std::cout << "account info: " << account.v << std::endl << std::endl;

    // // auto resp =
    // api.new_order("BTCUSDT",binapi::e_side::buy,binapi::e_type::market,binapi::e_time::FOK,binapi::e_trade_resp_type::FULL
    // //                          ,"0.001",nullptr,"myid",nullptr,nullptr);

    // // std::cout << "new order err " << resp.errmsg << std::endl;
    // // std::cout << "new order " << resp.reply << std::endl;

    // auto orders = api.all_orders("BTCUSDT");
    // std::cout << "order error" << orders.errmsg << std::endl;
    // std::cout << "orders " << orders.reply << std::endl;
    // for (auto &o : orders.v.orders) {
    //     std::cout << o.first << std::endl;
    //     for(auto &oi : o.second) {
    //         std::cout << oi.clientOrderId << " " << oi.orderId << " " << oi.cummulativeQuoteQty
    //         << " " << oi.executedQty; std::cout << " " << oi.icebergQty << " " << oi.isWorking <<
    //         " " << oi.origQty << " " << oi.price << " "; std::cout << oi.stopPrice << " " <<
    //         oi.side << " " << oi.status << " " << oi.symbol << " " << oi.time << " "; std::cout
    //         << oi.timeInForce << " " << oi.updateTime << " " << oi.type << std::endl;
    //     }
    // }

    return EXIT_SUCCESS;
}
