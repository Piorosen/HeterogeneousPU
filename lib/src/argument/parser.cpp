

#include <string>
#include <utility>

#include <argument/option.h>
#include <args.hxx>

std::pair<bool, option> parser(int argc, char** argv) {
    args::ArgumentParser parser("인공지능 모델을 동시에 연산 수행하는 프로그램 입니다.\n", "아래의 옵션을 지정하여 인공지능 모델을 변경할 수 있습니다.");
    args::HelpFlag help(parser, "help", "넹넹 설정하시죠!", {'h', "help"});
    
    args::ValueFlag<std::string> model(parser, "model", "인공지능 모델이 저장되어 있는 폴더 명을 입력해 주세요.", {'m', "model"});
    args::ValueFlag<std::string> layout(parser, "layout", "연산을 수행하기 위한 가중치의 레이아웃을 입력해주세요.(nchw, nhwc)", {'l', "layout"});
    args::ValueFlag<std::string> image(parser, "image", "연산을 수행하기 위한 이미지 파일의 이름을 지정해주세요.", {'i', "image"});

    args::ValueFlag<int> channel(parser, "channel", "입력 레이어의 Channel을 입력해 주세요.", {'c', "channel"});
    args::ValueFlag<int> width(parser, "width", "입력 레이어의 Width를 입력해 주세요.", {'w', "width"});
    args::ValueFlag<int> height(parser, "height", "입력 레이어의 Height를 입력해 주세요.", {'h', "height"});
    args::ValueFlag<int> batch(parser, "num", "입력 레이어의 Batch를 입력해 주세요.", {'n', "num"});

    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help)
    {
        std::cout << parser;
        return std::make_pair(false, option{});
    }
    catch (args::ParseError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return std::make_pair(false, option{});
    }
    catch (args::ValidationError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return std::make_pair(false, option{});
    }

    if (!model) { 
        std::cerr << "model 값이 설정이 되지 않았습니다.";
        std::cerr << parser;
        return std::make_pair(false, option{});
    }
    if (!image) { 
        std::cerr << "image 값이 설정이 되지 않았습니다.";
        std::cerr << parser;
        return std::make_pair(false, option{});
    }
    if (!layout) { 
        std::cerr << "layout 값이 설정이 되지 않았습니다.";
        std::cerr << parser;
        return std::make_pair(false, option{});
    }
    if (!height) { 
        std::cerr << "height 값이 설정이 되지 않았습니다.";
        std::cerr << parser;
        return std::make_pair(false, option{});
    }
    if (!channel) { 
        std::cerr << "channel 값이 설정이 되지 않았습니다.";
        std::cerr << parser;
        return std::make_pair(false, option{});
    }
    if (!width) { 
        std::cerr << "width 값이 설정이 되지 않았습니다.";
        std::cerr << parser;
        return std::make_pair(false, option{});
    }
    if (!batch) { 
        std::cerr << "batch 값이 설정이 되지 않았습니다.";
        std::cerr << parser;
        return std::make_pair(false, option{});
    }
    
    std::cout << "Argument Result : \n";
    std::cout << "model : " << args::get(model) << "\n";
    std::cout << "image : " << args::get(image) << "\n";
    std::cout << "layout : " << args::get(layout) << "\n";
    std::cout << "batch : " << args::get(batch) << "\n";
    std::cout << "channel : " << args::get(channel) << "\n";
    std::cout << "height : " << args::get(height) << "\n";
    std::cout << "width : " << args::get(width) << "\n\n\n";

    return std::make_pair(true, option {
        args::get(model),
        args::get(image),
        args::get(layout),
        args::get(channel),
        args::get(width),
        args::get(height),
        args::get(batch),
     });
}