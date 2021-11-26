#include "output.h"

#include <filesystem>
#include <fstream>

void OutputWriter::prepare_outdir() {
    if(!std::filesystem::is_directory(m_outdir)) {
        std::filesystem::create_directory(m_outdir);
    }
}

std::string OutputWriter::filepath(std::string filename) {
    return m_outdir + "/" + filename + ".csv";
}

OutputWriter::OutputWriter() {
    prepare_outdir();
}

void OutputWriter::set(OutputType ot) {
    switch(ot) {
        case OutputType::Fund: m_write_funds = true; break;
        case OutputType::Order: m_write_orders = true; break;
        case OutputType::PNL: m_write_pnls = true; break;
        case OutputType::Indicator: m_write_indicators = true; break;
        case OutputType::Price: m_write_prices = true; break;
    }
}

void OutputWriter::write_funds(std::stringstream &&ss) {
    if(!m_write_funds) return;
    std::ofstream fs(filepath("funds"));

    fs << ss.rdbuf();
}

void OutputWriter::write_orders(std::stringstream &&ss) {
    if(!m_write_funds) return;
    std::ofstream fs(filepath("orders"));

    fs << ss.rdbuf();
}

void OutputWriter::write_pnls(std::stringstream &&ss) {
    if(!m_write_funds) return;
    std::ofstream fs(filepath("pnls"));

    fs << ss.rdbuf();
}

void OutputWriter::write_indicators(std::stringstream &&ss) {
    if(!m_write_funds) return;
    std::ofstream fs(filepath("indicators"));

    fs << ss.rdbuf();
}

void OutputWriter::write_prices(std::stringstream &&ss) {
    if(!m_write_funds) return;
    std::ofstream fs(filepath("prices"));

    fs << ss.rdbuf();
}