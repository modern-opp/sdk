#include "ast/ast.hpp"

NodeBase::NodeBase(yy::location l) noexcept: location_(l) {}

NodeBase::~NodeBase() {}

yy::location NodeBase::location() const noexcept {
    return location_;
}

BodyExpr::BodyExpr(yy::location l) noexcept: NodeBase(l) {}

BodyExpr::~BodyExpr() {}

Body::Body(yy::location l, std::vector<std::unique_ptr<BodyExpr>> &&expressions) noexcept
        : NodeBase(l), expressions_(std::move(expressions)) {}

Body::~Body() {}

const std::vector<std::unique_ptr<BodyExpr>> &Body::expressions() const noexcept {
    return expressions_;
}

void Body::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

Expr::Expr(yy::location l) noexcept: BodyExpr(l) {}

Expr::~Expr() {}

PrimaryExpr::PrimaryExpr(yy::location l) noexcept: Expr(l) {}

PrimaryExpr::~PrimaryExpr() {}

BooleanLiteralExpr::BooleanLiteralExpr(yy::location l, bool value) noexcept: PrimaryExpr(l), value_(value) {}

BooleanLiteralExpr::~BooleanLiteralExpr() {}

bool BooleanLiteralExpr::value() const noexcept {
    return value_;
}

void BooleanLiteralExpr::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

IntegerLiteralExpr::IntegerLiteralExpr(yy::location l, int value) noexcept: PrimaryExpr(l), value_(value) {}

IntegerLiteralExpr::~IntegerLiteralExpr() {}

int IntegerLiteralExpr::value() const {
    return value_;
}

void IntegerLiteralExpr::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

RealLiteralExpr::RealLiteralExpr(yy::location l, double value) noexcept: PrimaryExpr(l), value_(value) {}

RealLiteralExpr::~RealLiteralExpr() {}

double RealLiteralExpr::value() const noexcept {
    return value_;
}

void RealLiteralExpr::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

StringLiteralExpr::StringLiteralExpr(const yy::location &l, const std::string &value) noexcept: PrimaryExpr(l),
                                                                                                value_(value) {}

StringLiteralExpr::~StringLiteralExpr() {}

const std::string &StringLiteralExpr::value() const {
    return value_;
}

void StringLiteralExpr::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

ThisExpr::ThisExpr(yy::location l) noexcept: PrimaryExpr(l) {}

ThisExpr::~ThisExpr() {}

void ThisExpr::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

MemberAccessExpr::MemberAccessExpr(yy::location l) noexcept: Expr(l) {}

MemberAccessExpr::~MemberAccessExpr() {}

FieldAccessExpr::FieldAccessExpr(const yy::location &l, const std::string &name) noexcept: MemberAccessExpr(l), name_(name) {}

FieldAccessExpr::~FieldAccessExpr() {}

const std::string &FieldAccessExpr::name() const {
    return name_;
}

void FieldAccessExpr::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

MethodCallExpr::MethodCallExpr(const yy::location &l, const std::string &name,
                               std::vector<std::unique_ptr<Expr>> &&arguments)
        : MemberAccessExpr(l), name_(name), arguments_(std::move(arguments)) {}

MethodCallExpr::~MethodCallExpr() {}

const std::string &MethodCallExpr::name() const noexcept {
    return name_;
}

const std::vector<std::unique_ptr<Expr>> &MethodCallExpr::arguments() const noexcept {
    return arguments_;
}

void MethodCallExpr::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

MemberAccess::MemberAccess(yy::location l, std::unique_ptr<Expr> &&lhs,
                           std::unique_ptr<MemberAccessExpr> &&rhs) noexcept
        : MemberAccessExpr(l), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

MemberAccess::~MemberAccess() {}

const Expr *MemberAccess::lhs() const noexcept {
    return lhs_.get();
}

const MemberAccessExpr *MemberAccess::rhs() const noexcept {
    return rhs_.get();
}

void MemberAccess::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

Stmt::Stmt(yy::location l) noexcept: BodyExpr(l) {}

Stmt::~Stmt() {}

ReturnStmt::ReturnStmt(yy::location l, std::unique_ptr<Expr> &&expression) noexcept
        : Stmt(l), expression_(std::move(expression)) {}

ReturnStmt::~ReturnStmt() {}

const Expr *ReturnStmt::expression() const noexcept {
    return expression_.get();
}

void ReturnStmt::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

AssignmentStmt::AssignmentStmt(yy::location l, std::string name, std::unique_ptr<Expr> &&expression)
        : Stmt(l), name_(std::move(name)), expression_(std::move(expression)) {}

AssignmentStmt::~AssignmentStmt() {}

const std::string &AssignmentStmt::name() const noexcept {
    return name_;
}

const Expr *AssignmentStmt::expression() const noexcept {
    return expression_.get();
}

void AssignmentStmt::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

IfStmt::IfStmt(yy::location l, std::unique_ptr<Expr> &&condition, std::unique_ptr<Body> &&then_body,
               std::unique_ptr<Body> &&else_body) noexcept
        : Stmt(l), condition_(std::move(condition)), then_body_(std::move(then_body)),
          else_body_(std::move(else_body)) {}

IfStmt::~IfStmt() {}

const Expr *IfStmt::condition() const noexcept {
    return condition_.get();
}

const Body *IfStmt::then_body() const noexcept {
    return then_body_.get();
}

const Body *IfStmt::else_body() const noexcept {
    return else_body_.get();
}

void IfStmt::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

WhileStmt::WhileStmt(yy::location l, std::unique_ptr<Expr> &&condition, std::unique_ptr<Body> &&loop_body) noexcept
        : Stmt(l), condition_(std::move(condition)), loop_body_(std::move(loop_body)) {}

WhileStmt::~WhileStmt() {}

const Expr *WhileStmt::condition() const noexcept {
    return condition_.get();
}

const Body *WhileStmt::loop_body() const noexcept {
    return loop_body_.get();
}

void WhileStmt::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

MemberDeclarationExpr::MemberDeclarationExpr(yy::location l) noexcept: NodeBase(l) {}

MemberDeclarationExpr::~MemberDeclarationExpr() {}

MemberDeclaration::MemberDeclaration(yy::location l,
                                     std::vector<std::unique_ptr<MemberDeclarationExpr>> &&member_declarations) noexcept
        : NodeBase(l), member_declarations_(std::move(member_declarations)) {}

MemberDeclaration::~MemberDeclaration() {}

const std::vector<std::unique_ptr<MemberDeclarationExpr>> &MemberDeclaration::member_declarations() const noexcept {
    return member_declarations_;
}

void MemberDeclaration::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

ParameterDeclaration::ParameterDeclaration(yy::location l, std::string name, std::string type)
        : NodeBase(l), name_(std::move(name)), type_(std::move(type)) {}

ParameterDeclaration::~ParameterDeclaration() {}

const std::string &ParameterDeclaration::name() const noexcept {
    return name_;
}

const std::string &ParameterDeclaration::type() const noexcept {
    return type_;
}

void ParameterDeclaration::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

VariableDeclaration::VariableDeclaration(yy::location l, std::string name, std::unique_ptr<Expr> &&initializer)
        : MemberDeclarationExpr(l), BodyExpr(l), name_(std::move(name)), initializer_(std::move(initializer)) {}

VariableDeclaration::~VariableDeclaration() {}

const std::string &VariableDeclaration::name() const noexcept {
    return name_;
}

const Expr *VariableDeclaration::initializer() const noexcept {
    return initializer_.get();
}

void VariableDeclaration::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

ConstructorDeclaration::ConstructorDeclaration(yy::location l,
                                               std::vector<std::unique_ptr<ParameterDeclaration>> &&parameters) noexcept
        : MemberDeclarationExpr(l), parameters_(std::move(parameters)) {}

ConstructorDeclaration::~ConstructorDeclaration() {}

const std::vector<std::unique_ptr<ParameterDeclaration>> &ConstructorDeclaration::parameters() const noexcept {
    return parameters_;
}

void ConstructorDeclaration::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

ConstructorDefinition::ConstructorDefinition(yy::location l, std::unique_ptr<ConstructorDeclaration> &&header,
                                             std::unique_ptr<Body> &&body) noexcept
        : MemberDeclarationExpr(l), header_(std::move(header)), body_(std::move(body)) {}

ConstructorDefinition::~ConstructorDefinition() {}

const ConstructorDeclaration *ConstructorDefinition::header() const noexcept {
    return header_.get();
}

const Body *ConstructorDefinition::body() const noexcept {
    return body_.get();
}

void ConstructorDefinition::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

MethodDeclaration::MethodDeclaration(yy::location l, std::string name,
                                     std::vector<std::unique_ptr<ParameterDeclaration>> &&parameters,
                                     std::string return_type)
        : MemberDeclarationExpr(l), name_(std::move(name)), parameters_(std::move(parameters)),
          return_type_(std::move(return_type)) {}

MethodDeclaration::~MethodDeclaration() {}

const std::string &MethodDeclaration::name() const noexcept {
    return name_;
}

const std::vector<std::unique_ptr<ParameterDeclaration>> &MethodDeclaration::parameters() const noexcept {
    return parameters_;
}

const std::string &MethodDeclaration::return_type() const noexcept {
    return return_type_;
}

void MethodDeclaration::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

MethodDefinition::MethodDefinition(yy::location l, std::unique_ptr<MethodDeclaration> &&header,
                                   std::unique_ptr<Body> &&body) noexcept
        : MemberDeclarationExpr(l), header_(std::move(header)), body_(std::move(body)) {}

MethodDefinition::~MethodDefinition() {}

const MethodDeclaration *MethodDefinition::header() const noexcept {
    return header_.get();
}

const Body *MethodDefinition::body() const noexcept {
    return body_.get();
}

void MethodDefinition::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

ProgramDeclarationExpr::ProgramDeclarationExpr(yy::location l) noexcept: NodeBase(l) {}

ProgramDeclarationExpr::~ProgramDeclarationExpr() {}

ProgramDeclaration::ProgramDeclaration(yy::location l,
                                       std::vector<std::unique_ptr<ProgramDeclarationExpr>> &&class_declarations) noexcept
        : NodeBase(l), class_declarations_(std::move(class_declarations)) {}

ProgramDeclaration::~ProgramDeclaration() {}

const std::vector<std::unique_ptr<ProgramDeclarationExpr>> &ProgramDeclaration::class_declarations() const noexcept {
    return class_declarations_;
}

void ProgramDeclaration::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

ClassDeclaration::ClassDeclaration(yy::location l, std::string name, std::string parent)
        : ProgramDeclarationExpr(l), name_(std::move(name)), parent_(std::move(parent)) {}

ClassDeclaration::~ClassDeclaration() {}

const std::string &ClassDeclaration::name() const noexcept {
    return name_;
}

const std::string &ClassDeclaration::parent() const noexcept {
    return parent_;
}

void ClassDeclaration::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

ClassDefinition::ClassDefinition(yy::location l, std::unique_ptr<ClassDeclaration> &&header,
                                 std::unique_ptr<MemberDeclaration> &&body) noexcept
        : ProgramDeclarationExpr(l), header_(std::move(header)), body_(std::move(body)) {}

ClassDefinition::~ClassDefinition() {}

const ClassDeclaration *ClassDefinition::header() const noexcept {
    return header_.get();
}

const MemberDeclaration *ClassDefinition::body() const noexcept {
    return body_.get();
}

void ClassDefinition::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

Program::Program(yy::location l, std::unique_ptr<ProgramDeclaration> &&class_declarations,
                 std::unique_ptr<Expr> &&main_class) noexcept
        : NodeBase(l), class_declarations_(std::move(class_declarations)), main_class_(std::move(main_class)) {}

Program::~Program() {}

const ProgramDeclaration *Program::class_declarations() const noexcept {
    return class_declarations_.get();
}

const Expr *Program::main_class() const noexcept {
    return main_class_.get();
}

void Program::do_accept(VisitorBase &visitor) const noexcept {
    visitor(*this);
}

VisitorBase::~VisitorBase() {}

Visitor<void>::~Visitor() {}

void Visitor<void>::result() const {}
