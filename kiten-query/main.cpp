#include <cstdio>
#include <string>
#include <vector>

#include <reproc++/reproc.hpp>
#include <reproc++/drain.hpp>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::fprintf(stderr, "");
        return 1;
    }

    reproc::process process;
    reproc::options options;
    options.stop = {
        { reproc::stop::noop, reproc::milliseconds(0) },
        { reproc::stop::terminate, reproc::milliseconds(5000) },
        { reproc::stop::kill, reproc::milliseconds(2000) },
    };
    options.redirect.err.type = reproc::redirect::pipe;

    std::vector<std::string> args = {
#if DEBUG_BUILD
        "./bin/kiten-query-helper",
#else
        CMAKE_INSTALL_PREFIX "/libexec/kiten-query-helper",
#endif
        argv[1],
    };

    std::error_code ec = process.start(args, options);
    if (ec)
    {
        std::fprintf(stderr, "query process failed -> %i: %s\n", ec.value(), ec.message().c_str());
        std::fprintf(stderr, "query process binary: %s\n", args.at(0).c_str());
        return 1;
    }

    // fetch process stdout and stderr (but ignore stderr)
    std::string output, error;
    reproc::sink::string sink_out(output);
    reproc::sink::string sink_err(error);
    ec = reproc::drain(process, sink_out, sink_err);

    options.stop.first = { reproc::stop::wait, reproc::milliseconds(10000) };

    int status = 0;
    std::tie(status, ec) = process.stop(options.stop);

    if (status == 5)
    {
        std::printf("検索結果はありませんでした。\n");
    }
    else
    {
        // only print stdout, we don't care about stderr since it contains useless crap from kf.i18n
        std::printf("%s\n", output.c_str());
    }

    return 0;
}
