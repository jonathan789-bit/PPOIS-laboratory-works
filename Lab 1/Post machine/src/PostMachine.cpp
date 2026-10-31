#include "PostMachine.h"

#include <istream>
#include <limits>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <utility>

PostInstruction PostInstruction::write0() { return {PostOp::Write0, 0}; }
PostInstruction PostInstruction::write1() { return {PostOp::Write1, 0}; }
PostInstruction PostInstruction::left() { return {PostOp::MoveLeft, 0}; }
PostInstruction PostInstruction::right() { return {PostOp::MoveRight, 0}; }
PostInstruction PostInstruction::jumpIfZero(std::size_t t) { return {PostOp::JumpIfZero, t}; }
PostInstruction PostInstruction::jumpIfOne(std::size_t t) { return {PostOp::JumpIfOne, t}; }
PostInstruction PostInstruction::halt() { return {PostOp::Halt, 0}; }

PostMachine::PostMachine(std::vector<PostInstruction> program)
    : program_(std::move(program)) {
    validateProgram();
}

void PostMachine::validateProgram() const {
    for (const auto& instruction : program_) {
        if ((instruction.op == PostOp::JumpIfZero || instruction.op == PostOp::JumpIfOne)
            && instruction.target > program_.size()) {
            throw std::invalid_argument("Jump target is outside the program.");
        }
    }
}

bool PostMachine::step() {
    if (halted_) return true;
    if (state_ >= program_.size()) { halted_ = true; return true; }

    const PostInstruction instruction = program_[state_];
    switch (instruction.op) {
    case PostOp::Write0:
        tape_[head_] = false;
        ++state_;
        break;
    case PostOp::Write1:
        tape_[head_] = true;
        ++state_;
        break;
    case PostOp::MoveLeft:
        if (head_ == std::numeric_limits<std::int64_t>::min())
            throw std::overflow_error("Tape head position underflow.");
        --head_;
        ++state_;
        break;
    case PostOp::MoveRight:
        if (head_ == std::numeric_limits<std::int64_t>::max())
            throw std::overflow_error("Tape head position overflow.");
        ++head_;
        ++state_;
        break;
    case PostOp::JumpIfZero:
        state_ = tapeAt(head_) ? state_ + 1 : instruction.target;
        break;
    case PostOp::JumpIfOne:
        state_ = tapeAt(head_) ? instruction.target : state_ + 1;
        break;
    case PostOp::Halt:
        halted_ = true;
        break;
    }
    if (state_ >= program_.size() && !halted_) halted_ = true;
    return halted_;
}

PostMachine::RunResult PostMachine::run(std::size_t maxSteps) {
    std::size_t steps = 0;
    while (!halted_ && steps < maxSteps) {
        step();
        ++steps;
    }
    return {halted_, steps};
}

bool PostMachine::tapeAt(std::int64_t p) const noexcept {
    const auto it = tape_.find(p);
    return it != tape_.end() && it->second;
}
void PostMachine::setTapeAt(std::int64_t p, bool value) {
    if (value) tape_[p] = true;
    else tape_.erase(p);
}
std::int64_t PostMachine::headPosition() const noexcept { return head_; }
std::size_t PostMachine::currentState() const noexcept { return state_; }
bool PostMachine::isHalted() const noexcept { return halted_; }
std::size_t PostMachine::programSize() const noexcept { return program_.size(); }
const std::vector<PostInstruction>& PostMachine::program() const noexcept { return program_; }

bool PostMachine::operator==(const PostMachine& m) const noexcept {
    return program_.size() == m.program_.size()
        && state_ == m.state_ && head_ == m.head_ && halted_ == m.halted_
        && tape_ == m.tape_
        && [&]() {
            for (std::size_t i = 0; i < program_.size(); ++i)
                if (program_[i].op != m.program_[i].op ||
                    program_[i].target != m.program_[i].target) return false;
            return true;
        }();
}
bool PostMachine::operator!=(const PostMachine& m) const noexcept { return !(*this == m); }

static const char* opName(PostOp op) {
    switch (op) {
    case PostOp::Write0: return "WRITE0";
    case PostOp::Write1: return "WRITE1";
    case PostOp::MoveLeft: return "LEFT";
    case PostOp::MoveRight: return "RIGHT";
    case PostOp::JumpIfZero: return "JZ";
    case PostOp::JumpIfOne: return "J1";
    case PostOp::Halt: return "HALT";
    }
    return "HALT";
}
std::ostream& operator<<(std::ostream& out, const PostMachine& m) {
    out << "POSTMACHINE 1\n"
        << "HEAD " << m.head_ << "\n"
        << "STATE " << m.state_ << "\n"
        << "HALTED " << (m.halted_ ? 1 : 0) << "\n"
        << "TAPE " << m.tape_.size() << "\n";
    for (const auto& cell : m.tape_)
        out << cell.first << ' ' << (cell.second ? 1 : 0) << "\n";
    out << "PROGRAM " << m.program_.size() << "\n";
    for (const auto& instruction : m.program_) {
        out << opName(instruction.op);
        if (instruction.op == PostOp::JumpIfZero || instruction.op == PostOp::JumpIfOne)
            out << ' ' << instruction.target;
        out << "\n";
    }
    return out;
}

std::istream& operator>>(std::istream& in, PostMachine& m) {
    std::string tag, op;
    int version = 0, halted = 0, bit = 0;
    std::int64_t head = 0, position = 0;
    std::size_t state = 0, tapeCount = 0, programCount = 0;
    if (!(in >> tag >> version) || tag != "POSTMACHINE" || version != 1) {
        in.setstate(std::ios::failbit); return in;
    }
    if (!(in >> tag >> head) || tag != "HEAD" ||
        !(in >> tag >> state) || tag != "STATE" ||
        !(in >> tag >> halted) || tag != "HALTED" || (halted != 0 && halted != 1) ||
        !(in >> tag >> tapeCount) || tag != "TAPE") {
        in.setstate(std::ios::failbit); return in;
    }
    std::map<std::int64_t, bool> tape;
    for (std::size_t i = 0; i < tapeCount; ++i) {
        if (!(in >> position >> bit) || (bit != 0 && bit != 1)) {
            in.setstate(std::ios::failbit); return in;
        }
        if (bit == 1) tape[position] = true;
    }
    if (!(in >> tag >> programCount) || tag != "PROGRAM") {
        in.setstate(std::ios::failbit); return in;
    }
    std::vector<PostInstruction> program;
    for (std::size_t i = 0; i < programCount; ++i) {
        if (!(in >> op)) { in.setstate(std::ios::failbit); return in; }
        if (op == "WRITE0") program.push_back(PostInstruction::write0());
        else if (op == "WRITE1") program.push_back(PostInstruction::write1());
        else if (op == "LEFT") program.push_back(PostInstruction::left());
        else if (op == "RIGHT") program.push_back(PostInstruction::right());
        else if (op == "HALT") program.push_back(PostInstruction::halt());
        else if (op == "JZ" || op == "J1") {
            std::size_t target;
            if (!(in >> target)) { in.setstate(std::ios::failbit); return in; }
            program.push_back(op == "JZ" ? PostInstruction::jumpIfZero(target)
                                          : PostInstruction::jumpIfOne(target));
        } else {
            in.setstate(std::ios::failbit); return in;
        }
    }
    try {
        PostMachine temp(std::move(program));
        temp.head_ = head; temp.state_ = state; temp.halted_ = halted != 0;
        temp.tape_ = std::move(tape);
        if (state > temp.program_.size()) throw std::invalid_argument("Invalid state.");
        m = std::move(temp);
    } catch (...) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

PostMachine PostMachine::parse(const std::string& text) {
    std::istringstream in(text);
    PostMachine result;
    if (!(in >> result)) throw std::invalid_argument("Invalid PostMachine text.");
    in >> std::ws;
    if (!in.eof()) throw std::invalid_argument("Unexpected data after PostMachine.");
    return result;
}
