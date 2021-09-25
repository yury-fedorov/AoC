#include "intcode_computer.h"

using namespace std;

namespace intcode_computer {

    Number get( const Memory & memory, Number value, Mode mode, Number relative_base ) noexcept {
        switch (mode) {
            case Mode::Position:
                return value >= static_cast<Number>( memory.size() ) ? 0 : memory[value];
            case Mode::Immediate: return value;
            case Mode::Relative:  return relative_base + value;
        }
        assert( false ); // "unexpected mode type"
        return -1;
    }

    void set( Memory & memory, Number index, Number value ) noexcept {
        if ( static_cast<Number>( memory.size() ) < index ) {
            memory.resize(index + 1, 0);
        }
        memory[index] = value;
    }

    Mode get_mode(char mode) {
        Expects(mode == '0' || mode == '1' );
        return mode == '1' ? Mode::Immediate : Mode::Position;
    }

    constexpr Jump NextOp = {JumpType::Next, 0};

    Execute create_execute(Command command) {
        switch(command) {
            case Command::End: return [](Memory&, Queue&, Queue&, s::span<const Mode> , s::span<const Number>, Number & )
                { return Jump(JumpType::Absolute, 1'000'000 ); };
            case Command::Add:
            case Command::Mul:
            case Command::Equals:
            case Command::LessThan:
                return [command](Memory& memory, Queue&, Queue&, s::span<const Mode> mode,
                        s::span<const Number> args, Number & relative_base)
                {
                    const auto a = get(memory, args[0], mode[0], relative_base);
                    const auto b = get(memory, args[1], mode[1], relative_base);
                    Number r;
                    switch( command ) {
                        case Command::Add: r = a + b; break;
                        case Command::Mul: r = a * b; break;
                        case Command::Equals: r = a == b ? 1 : 0; break;
                        case Command::LessThan: r = a < b ? 1 : 0; break;
                        default: assert(false); // not expected
                    }
                    const auto ra = args[2];
                    set(memory, ra, r);
                    return NextOp;
                };
            case Command::JumpIfFalse:
            case Command::JumpIfTrue:
                return [command](Memory& memory, Queue&, Queue&, s::span<const Mode> mode,
                        s::span<const Number> args, Number & relative_base)
                {
                    const auto a = get(memory, args[0], mode[0], relative_base);
                    const auto b = get(memory, args[1], mode[1], relative_base);
                    const bool is_jump = (command == Command::JumpIfFalse) ? a == 0 : a != 0;
                    return is_jump ? Jump( JumpType::Absolute, b ) : NextOp;
                };

            case Command::In: return [](Memory& memory, Queue& in, Queue&, s::span<const Mode>,
                    s::span<const Number> args, Number & )
                {
                    set( memory, args[0], in.front() );
                    in.pop();
                    return NextOp;
                };
            case Command::Out: return [](Memory& memory, Queue&, Queue& out, s::span<const Mode> mode,
                    s::span<const Number> args, Number & relative_base )
                {
                    out.push( get(memory,  args[0], mode[0], relative_base ) );
                    return NextOp;
                };
            case Command::AdjustRelativeBase: return [](Memory& memory, Queue&, Queue&,
                    s::span<const Mode> mode, s::span<const Number> args, Number & relative_base )
                {
                    relative_base += get( memory, args[0], mode[0], relative_base );
                    return NextOp;
                };
        }
        throw runtime_error( fmt::format( "unexpected command type: {}", command ) );
    }

    std::shared_ptr<Operation> create_operation( s::span<const Number> & code ) {
        const auto & modes_code = code[0];
        const auto mc = div(modes_code, 100LL); // quot - modes, rem - command
        const auto command_code = mc.rem;
        const string str_modes = fmt::format( "{:03d}", mc.quot );
        const Modes modes = { get_mode(str_modes[2]), get_mode(str_modes[1]), get_mode(str_modes[0]) };
        const Command command = static_cast<Command>(command_code);
        Arguments args;
        switch ( command ) {
            case Command::End: break;
            case Command::Add:
            case Command::Mul:
            case Command::Equals:
            case Command::LessThan:
                args = { code[1], code[2], code[3] };
                break;
            case Command::JumpIfTrue:
            case Command::JumpIfFalse:
                args = { code[1], code[2] };
                break;
            case Command::In:
            case Command::Out:
            case Command::AdjustRelativeBase:
                args = { code[1] };
                break;
        }
        return shared_ptr<Operation>( new Operation( command, modes, args, create_execute(command) ) );
    }

    Memory load( const std::string & code )
    {
        const auto vs = split( code, ',' );
        return vs | rv::transform( [](const string & s) { return stoll(s); } ) | r::to_vector;
    }

    bool run( Memory memory, Queue& in, Queue& out ) {
        auto cur = 0;
        Number relative_base = 0;
        auto memory_span = s::span(memory);
        while ( cur >= 0 && cur < static_cast<int>( memory.size() ) )  {
            s::span<const Number> frame = memory_span.subspan( cur );
            const auto o = create_operation( frame );
            if ( o->command_ == Command::End ) return true;
            const auto jump = o->execute(memory, in, out, relative_base);
            cur = ( jump.first == JumpType::Absolute ) ? jump.second : ( cur + o->length() );
        }
        return false;
    }
}
