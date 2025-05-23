#ifndef AST_HPP
#define AST_HPP

#include "location.hh"

#include <memory>
#include <string>
#include <utility>
#include <vector>

class VisitorBase;

template <class T>
class Visitor;

class NodeBase {
public:
    virtual ~NodeBase() = 0;

    template <class T>
    T accept(Visitor<T> &visitor) const noexcept;

    yy::location location() const noexcept;

protected:
    NodeBase(yy::location l) noexcept;

    virtual void do_accept(VisitorBase &visitor) const noexcept = 0;

private:
    yy::location location_;
};

class BodyExpr : public NodeBase {
public:

    ~BodyExpr() override;

protected:
    BodyExpr(yy::location l) noexcept;
};

class Body : public NodeBase {
public:
    Body(yy::location l, std::vector<std::unique_ptr<BodyExpr>> &&expressions) noexcept;

    const std::vector<std::unique_ptr<BodyExpr>> &expressions() const noexcept;

    ~Body() override;

private:
    std::vector<std::unique_ptr<BodyExpr>> expressions_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class Expr : public BodyExpr {
public:
    ~Expr() override;


protected:
    Expr(yy::location l) noexcept;
};

class PrimaryExpr : public Expr {
public:

    ~PrimaryExpr() override;

protected:
    PrimaryExpr(yy::location l) noexcept;
};

class BooleanLiteralExpr : public PrimaryExpr {
public:
    BooleanLiteralExpr(yy::location l, bool value) noexcept;

    bool value() const noexcept;

    ~BooleanLiteralExpr() override;

private:
    bool value_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class IntegerLiteralExpr : public PrimaryExpr {
public:
    IntegerLiteralExpr(yy::location l, int value) noexcept;

    int value() const;

    ~IntegerLiteralExpr() override;

private:
    int value_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class RealLiteralExpr : public PrimaryExpr {
public:
    RealLiteralExpr(yy::location l, double value) noexcept;

    double value() const noexcept;

    ~RealLiteralExpr() override;

private:
    double value_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class StringLiteralExpr : public PrimaryExpr {
public:
    StringLiteralExpr(const yy::location &l, const std::string &value) noexcept;

    const std::string &value() const;

    ~StringLiteralExpr() override;

private:
    std::string value_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class ThisExpr : public PrimaryExpr {
public:
    ThisExpr(yy::location l) noexcept;

    ~ThisExpr() override;

private:
    void do_accept(VisitorBase &visitor) const noexcept override;
};

class MemberAccessExpr : public Expr {
public:

    ~MemberAccessExpr() override;

protected:
    MemberAccessExpr(yy::location l) noexcept;
};

class FieldAccessExpr : public MemberAccessExpr {
public:
    FieldAccessExpr(const yy::location &l, const std::string &name) noexcept;

    const std::string &name() const;

    ~FieldAccessExpr() override;

private:
    std::string name_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class MethodCallExpr : public MemberAccessExpr {
public:
    MethodCallExpr(const yy::location &l, const std::string &name, std::vector<std::unique_ptr<Expr>> &&arguments);

    const std::string &name() const noexcept;

    const std::vector<std::unique_ptr<Expr>> &arguments() const noexcept;

    ~MethodCallExpr() override;

private:
    std::string name_;
    std::vector<std::unique_ptr<Expr>> arguments_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class MemberAccess : public MemberAccessExpr {
public:
    MemberAccess(yy::location l, std::unique_ptr<Expr> &&lhs, std::unique_ptr<MemberAccessExpr> &&rhs) noexcept;

    const Expr *lhs() const noexcept;

    const MemberAccessExpr *rhs() const noexcept;

    ~MemberAccess() override;

private:
    std::unique_ptr<Expr> lhs_;
    std::unique_ptr<MemberAccessExpr> rhs_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class Stmt : public BodyExpr {
public:

    ~Stmt() override;

protected:
    Stmt(yy::location l) noexcept;
};

class ReturnStmt : public Stmt {
public:
    ReturnStmt(yy::location l, std::unique_ptr<Expr> &&expression) noexcept;

    const Expr *expression() const noexcept;

    ~ReturnStmt() override;

private:
    std::unique_ptr<Expr> expression_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class AssignmentStmt : public Stmt {
public:
    AssignmentStmt(yy::location l, std::string name, std::unique_ptr<Expr> &&expression);

    const std::string &name() const noexcept;

    const Expr *expression() const noexcept;

    ~AssignmentStmt() override;

private:
    std::string name_;
    std::unique_ptr<Expr> expression_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class IfStmt : public Stmt {
public:
    IfStmt(yy::location l, std::unique_ptr<Expr> &&condition, std::unique_ptr<Body> &&then_body, std::unique_ptr<Body> &&else_body = nullptr) noexcept;

    const Expr *condition() const noexcept;

    const Body *then_body() const noexcept;

    const Body *else_body() const noexcept;

    ~IfStmt() override;

private:
    std::unique_ptr<Expr> condition_;
    std::unique_ptr<Body> then_body_;
    std::unique_ptr<Body> else_body_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class WhileStmt : public Stmt {
public:
    WhileStmt(yy::location l, std::unique_ptr<Expr> &&condition, std::unique_ptr<Body> &&loop_body) noexcept;

    const Expr *condition() const noexcept;

    const Body *loop_body() const noexcept;

    ~WhileStmt() override;

private:
    std::unique_ptr<Expr> condition_;
    std::unique_ptr<Body> loop_body_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class MemberDeclarationExpr : public NodeBase {
public:

    ~MemberDeclarationExpr() override;

protected:
    MemberDeclarationExpr(yy::location l) noexcept;
};

class MemberDeclaration : public NodeBase {
public:
    MemberDeclaration(yy::location l, std::vector<std::unique_ptr<MemberDeclarationExpr>> &&member_declarations) noexcept;

    const std::vector<std::unique_ptr<MemberDeclarationExpr>> &member_declarations() const noexcept;

    ~MemberDeclaration() override;

private:
    std::vector<std::unique_ptr<MemberDeclarationExpr>> member_declarations_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class ParameterDeclaration : public NodeBase {
public:
    ParameterDeclaration(yy::location l, std::string name, std::string type);

    const std::string &name() const noexcept;

    const std::string &type() const noexcept;

    ~ParameterDeclaration() override;

private:
    std::string name_;
    std::string type_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class VariableDeclaration : public MemberDeclarationExpr, public BodyExpr {
public:
    VariableDeclaration(yy::location l, std::string name, std::unique_ptr<Expr> &&initializer);

    const std::string &name() const noexcept;

    const Expr *initializer() const noexcept;

    ~VariableDeclaration() override;

private:
    std::string name_;
    std::unique_ptr<Expr> initializer_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class ConstructorDeclaration : public MemberDeclarationExpr {
public:
    ConstructorDeclaration(yy::location l, std::vector<std::unique_ptr<ParameterDeclaration>> &&parameters) noexcept;

    const std::vector<std::unique_ptr<ParameterDeclaration>> &parameters() const noexcept;

    ~ConstructorDeclaration() override;

private:
    std::vector<std::unique_ptr<ParameterDeclaration>> parameters_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class ConstructorDefinition : public MemberDeclarationExpr {
public:
    ConstructorDefinition(yy::location l, std::unique_ptr<ConstructorDeclaration> &&header, std::unique_ptr<Body> &&body) noexcept;

    const ConstructorDeclaration *header() const noexcept;

    const Body *body() const noexcept;

    ~ConstructorDefinition() override;

private:
    std::unique_ptr<ConstructorDeclaration> header_;
    std::unique_ptr<Body> body_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class MethodDeclaration : public MemberDeclarationExpr {
public:
    MethodDeclaration(yy::location l, std::string name, std::vector<std::unique_ptr<ParameterDeclaration>> &&parameters, std::string return_type = "");

    const std::string &name() const noexcept;

    const std::vector<std::unique_ptr<ParameterDeclaration>> &parameters() const noexcept;

    const std::string &return_type() const noexcept;

    ~MethodDeclaration() override;

private:
    std::string name_;
    std::vector<std::unique_ptr<ParameterDeclaration>> parameters_;
    std::string return_type_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class MethodDefinition : public MemberDeclarationExpr {
public:
    MethodDefinition(yy::location l, std::unique_ptr<MethodDeclaration> &&header, std::unique_ptr<Body> &&body) noexcept;

    const MethodDeclaration *header() const noexcept;

    const Body *body() const noexcept;

    ~MethodDefinition() override;

private:
    std::unique_ptr<MethodDeclaration> header_;
    std::unique_ptr<Body> body_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class ProgramDeclarationExpr : public NodeBase {
public:

    ~ProgramDeclarationExpr() override;

protected:
    ProgramDeclarationExpr(yy::location l) noexcept;
};

class ProgramDeclaration : public NodeBase {
public:
    ProgramDeclaration(yy::location l, std::vector<std::unique_ptr<ProgramDeclarationExpr>> &&class_declarations) noexcept;

    const std::vector<std::unique_ptr<ProgramDeclarationExpr>> &class_declarations() const noexcept;

    ~ProgramDeclaration() override;

private:
    std::vector<std::unique_ptr<ProgramDeclarationExpr>> class_declarations_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class ClassDeclaration : public ProgramDeclarationExpr {
public:
    ClassDeclaration(yy::location l, std::string name, std::string parent = "");

    const std::string &name() const noexcept;

    const std::string &parent() const noexcept;

    ~ClassDeclaration() override;

private:
    std::string name_;
    std::string parent_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class ClassDefinition : public ProgramDeclarationExpr {
public:
    ClassDefinition(yy::location l, std::unique_ptr<ClassDeclaration> &&header, std::unique_ptr<MemberDeclaration> &&body) noexcept;

    const ClassDeclaration *header() const noexcept;

    const MemberDeclaration *body() const noexcept;

    ~ClassDefinition() override;

private:
    std::unique_ptr<ClassDeclaration> header_;
    std::unique_ptr<MemberDeclaration> body_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class Program : public NodeBase {
public:
    Program(yy::location l, std::unique_ptr<ProgramDeclaration> &&class_declarations, std::unique_ptr<Expr> &&main_class) noexcept;

    const ProgramDeclaration *class_declarations() const noexcept;

    const Expr *main_class() const noexcept;

    ~Program() override;

private:
    std::unique_ptr<ProgramDeclaration> class_declarations_;
    std::unique_ptr<Expr> main_class_;

    void do_accept(VisitorBase &visitor) const noexcept override;
};

class VisitorBase {
public:
    virtual void operator()(const BooleanLiteralExpr &boolean_literal_expr) = 0;

    virtual void operator()(const IntegerLiteralExpr &integer_literal_expr) = 0;

    virtual void operator()(const RealLiteralExpr &real_literal_expr) = 0;

    virtual void operator()(const StringLiteralExpr &string_literal_expr) = 0;

    virtual void operator()(const ThisExpr &this_expr) = 0;

    virtual void operator()(const FieldAccessExpr &field_access_expr) = 0;

    virtual void operator()(const MethodCallExpr &method_call_expr) = 0;

    virtual void operator()(const MemberAccess &member_access) = 0;

    virtual void operator()(const Body &body) = 0;

    virtual void operator()(const ReturnStmt &return_stmt) = 0;

    virtual void operator()(const AssignmentStmt &assignment_stmt) = 0;

    virtual void operator()(const IfStmt &if_stmt) = 0;

    virtual void operator()(const WhileStmt &while_stmt) = 0;

    virtual void operator()(const MemberDeclaration &member_declaration) = 0;

    virtual void operator()(const ParameterDeclaration &parameter_declaration) = 0;

    virtual void operator()(const VariableDeclaration &variable_declaration) = 0;

    virtual void operator()(const ConstructorDeclaration &constructor_declaration) = 0;

    virtual void operator()(const ConstructorDefinition &constructor_definition) = 0;

    virtual void operator()(const MethodDeclaration &method_declaration) = 0;

    virtual void operator()(const MethodDefinition &method_definition) = 0;

    virtual void operator()(const ProgramDeclaration &program_declaration) = 0;

    virtual void operator()(const ClassDeclaration &class_declaration) = 0;

    virtual void operator()(const ClassDefinition &class_definition) = 0;

    virtual void operator()(const Program &program) = 0;

    virtual ~VisitorBase();
};

template <class T>
class Visitor : public VisitorBase {
public:
    T result() const;

    virtual ~Visitor();

private:
    T result_;
};

template <>
class Visitor<void> : public VisitorBase {
public:
    void result() const;

    virtual ~Visitor();
};

template <class T>
T NodeBase::accept(Visitor<T> &visitor) const noexcept {
    do_accept(visitor);
    return visitor.result();
}

template <class T>
Visitor<T>::~Visitor() {}

template <class T>
T Visitor<T>::result() const {
    return result_;
}

#endif
