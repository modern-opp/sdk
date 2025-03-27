#ifndef AST_HPP
#define AST_HPP

#include "location.hh"

#include <memory>
#include <string>
#include <utility>
#include <vector>

class Visitor;

class NodeBase {
public:
    virtual ~NodeBase() = 0;

    yy::location location() const noexcept;

    virtual void accept(Visitor &visitor) const noexcept = 0;

protected:
    NodeBase(yy::location l) noexcept;

private:
    yy::location location_;
};

class BodyExpr : public NodeBase {
public:
    void accept(Visitor &visitor) const noexcept override;

    ~BodyExpr() override;

protected:
    BodyExpr(yy::location l) noexcept;
};

class Body : public NodeBase {
public:
    Body(yy::location l, std::vector<std::unique_ptr<BodyExpr>> &&expressions) noexcept;

    const std::vector<std::unique_ptr<BodyExpr>> &expressions() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~Body() override;

private:
    std::vector<std::unique_ptr<BodyExpr>> expressions_;
};

class Expr : public BodyExpr {
public:
    ~Expr() override;

    void accept(Visitor &visitor) const noexcept override;

protected:
    Expr(yy::location l) noexcept;
};

class PrimaryExpr : public Expr {
public:
    void accept(Visitor &visitor) const noexcept override;

    ~PrimaryExpr() override;

protected:
    PrimaryExpr(yy::location l) noexcept;
};

class BooleanLiteralExpr : public PrimaryExpr {
public:
    BooleanLiteralExpr(yy::location l, bool value) noexcept;

    bool value() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~BooleanLiteralExpr() override;

private:
    bool value_;
};

class IntegerLiteralExpr : public PrimaryExpr {
public:
    IntegerLiteralExpr(yy::location l, int value) noexcept;

    int value() const;

    void accept(Visitor &visitor) const noexcept override;

    ~IntegerLiteralExpr() override;

private:
    int value_;
};

class RealLiteralExpr : public PrimaryExpr {
public:
    RealLiteralExpr(yy::location l, double value) noexcept;

    double value() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~RealLiteralExpr() override;

private:
    double value_;
};

class StringLiteralExpr : public PrimaryExpr {
public:
    StringLiteralExpr(const yy::location &l, const std::string &value) noexcept;

    const std::string &value() const;

    void accept(Visitor &visitor) const noexcept override;

    ~StringLiteralExpr() override;

private:
    std::string value_;
};

class ThisExpr : public PrimaryExpr {
public:
    ThisExpr(yy::location l) noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~ThisExpr() override;
};

class MemberAccessExpr : public Expr {
public:
    void accept(Visitor &visitor) const noexcept override;

    ~MemberAccessExpr() override;

protected:
    MemberAccessExpr(yy::location l) noexcept;
};

class FieldAccessExpr : public MemberAccessExpr {
public:
    FieldAccessExpr(const yy::location &l, const std::string &name) noexcept;

    const std::string &name() const;

    void accept(Visitor &visitor) const noexcept override;

    ~FieldAccessExpr() override;

private:
    std::string name_;
};

class MethodCallExpr : public MemberAccessExpr {
public:
    MethodCallExpr(const yy::location &l, const std::string &name, std::vector<std::unique_ptr<Expr>> &&arguments);

    const std::string &name() const noexcept;

    const std::vector<std::unique_ptr<Expr>> &arguments() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~MethodCallExpr() override;

private:
    std::string name_;
    std::vector<std::unique_ptr<Expr>> arguments_;
};

class MemberAccess : public MemberAccessExpr {
public:
    MemberAccess(yy::location l, std::unique_ptr<Expr> &&lhs, std::unique_ptr<MemberAccessExpr> &&rhs) noexcept;

    const Expr *lhs() const noexcept;

    const MemberAccessExpr *rhs() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~MemberAccess() override;

private:
    std::unique_ptr<Expr> lhs_;
    std::unique_ptr<MemberAccessExpr> rhs_;
};

class Stmt : public BodyExpr {
public:
    void accept(Visitor &visitor) const noexcept override;

    ~Stmt() override;

protected:
    Stmt(yy::location l) noexcept;
};

class ReturnStmt : public Stmt {
public:
    ReturnStmt(yy::location l, std::unique_ptr<Expr> &&expression) noexcept;

    const Expr *expression() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~ReturnStmt() override;

private:
    std::unique_ptr<Expr> expression_;
};

class AssignmentStmt : public Stmt {
public:
    AssignmentStmt(yy::location l, std::string name, std::unique_ptr<Expr> &&expression);

    const std::string &name() const noexcept;

    const Expr *expression() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~AssignmentStmt() override;

private:
    std::string name_;
    std::unique_ptr<Expr> expression_;
};

class IfStmt : public Stmt {
public:
    IfStmt(yy::location l, std::unique_ptr<Expr> &&condition, std::unique_ptr<Body> &&then_body, std::unique_ptr<Body> &&else_body = nullptr) noexcept;

    const Expr *condition() const noexcept;

    const Body *then_body() const noexcept;

    const Body *else_body() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~IfStmt() override;

private:
    std::unique_ptr<Expr> condition_;
    std::unique_ptr<Body> then_body_;
    std::unique_ptr<Body> else_body_;
};

class WhileStmt : public Stmt {
public:
    WhileStmt(yy::location l, std::unique_ptr<Expr> &&condition, std::unique_ptr<Body> &&loop_body) noexcept;

    const Expr *condition() const noexcept;

    const Body *loop_body() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~WhileStmt() override;

private:
    std::unique_ptr<Expr> condition_;
    std::unique_ptr<Body> loop_body_;
};

class MemberDeclarationExpr : public NodeBase {
public:
    void accept(Visitor &visitor) const noexcept override;

    ~MemberDeclarationExpr() override;

protected:
    MemberDeclarationExpr(yy::location l) noexcept;
};

class MemberDeclaration : public NodeBase {
public:
    MemberDeclaration(yy::location l, std::vector<std::unique_ptr<MemberDeclarationExpr>> &&member_declarations) noexcept;

    const std::vector<std::unique_ptr<MemberDeclarationExpr>> &member_declarations() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~MemberDeclaration() override;

private:
    std::vector<std::unique_ptr<MemberDeclarationExpr>> member_declarations_;
};

class ParameterDeclaration : public NodeBase {
public:
    ParameterDeclaration(yy::location l, std::string name, std::string type);

    const std::string &name() const noexcept;

    const std::string &type() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~ParameterDeclaration() override;

private:
    std::string name_;
    std::string type_;
};

class VariableDeclaration : public MemberDeclarationExpr, public BodyExpr {
public:
    VariableDeclaration(yy::location l, std::string name, std::unique_ptr<Expr> &&initializer);

    const std::string &name() const noexcept;

    const Expr *initializer() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~VariableDeclaration() override;

private:
    std::string name_;
    std::unique_ptr<Expr> initializer_;
};

class ConstructorDeclaration : public MemberDeclarationExpr {
public:
    ConstructorDeclaration(yy::location l, std::vector<std::unique_ptr<ParameterDeclaration>> &&parameters) noexcept;

    const std::vector<std::unique_ptr<ParameterDeclaration>> &parameters() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~ConstructorDeclaration() override;

private:
    std::vector<std::unique_ptr<ParameterDeclaration>> parameters_;
};

class ConstructorDefinition : public MemberDeclarationExpr {
public:
    ConstructorDefinition(yy::location l, std::unique_ptr<ConstructorDeclaration> &&header, std::unique_ptr<Body> &&body) noexcept;

    const ConstructorDeclaration *header() const noexcept;

    const Body *body() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~ConstructorDefinition() override;

private:
    std::unique_ptr<ConstructorDeclaration> header_;
    std::unique_ptr<Body> body_;
};

class MethodDeclaration : public MemberDeclarationExpr {
public:
    MethodDeclaration(yy::location l, std::string name, std::vector<std::unique_ptr<ParameterDeclaration>> &&parameters, std::string return_type = "");

    const std::string &name() const noexcept;

    const std::vector<std::unique_ptr<ParameterDeclaration>> &parameters() const noexcept;

    const std::string &return_type() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~MethodDeclaration() override;

private:
    std::string name_;
    std::vector<std::unique_ptr<ParameterDeclaration>> parameters_;
    std::string return_type_;
};

class MethodDefinition : public MemberDeclarationExpr {
public:
    MethodDefinition(yy::location l, std::unique_ptr<MethodDeclaration> &&header, std::unique_ptr<Body> &&body) noexcept;

    const MethodDeclaration *header() const noexcept;

    const Body *body() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~MethodDefinition() override;

private:
    std::unique_ptr<MethodDeclaration> header_;
    std::unique_ptr<Body> body_;
};

class ProgramDeclarationExpr : public NodeBase {
public:
    void accept(Visitor &visitor) const noexcept override;

    ~ProgramDeclarationExpr() override;

protected:
    ProgramDeclarationExpr(yy::location l) noexcept;
};

class ProgramDeclaration : public NodeBase {
public:
    ProgramDeclaration(yy::location l, std::vector<std::unique_ptr<ProgramDeclarationExpr>> &&class_declarations) noexcept;

    const std::vector<std::unique_ptr<ProgramDeclarationExpr>> &class_declarations() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~ProgramDeclaration() override;

private:
    std::vector<std::unique_ptr<ProgramDeclarationExpr>> class_declarations_;
};

class ClassDeclaration : public ProgramDeclarationExpr {
public:
    ClassDeclaration(yy::location l, std::string name, std::string parent = "");

    const std::string &name() const noexcept;

    const std::string &parent() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~ClassDeclaration() override;

private:
    std::string name_;
    std::string parent_;
};

class ClassDefinition : public ProgramDeclarationExpr {
public:
    ClassDefinition(yy::location l, std::unique_ptr<ClassDeclaration> &&header, std::unique_ptr<MemberDeclaration> &&body) noexcept;

    const ClassDeclaration *header() const noexcept;

    const MemberDeclaration *body() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~ClassDefinition() override;

private:
    std::unique_ptr<ClassDeclaration> header_;
    std::unique_ptr<MemberDeclaration> body_;
};

class Program : public NodeBase {
public:
    Program(yy::location l, std::unique_ptr<ProgramDeclaration> &&class_declarations, std::unique_ptr<MethodCallExpr> &&main_class) noexcept;

    const ProgramDeclaration *class_declarations() const noexcept;

    const MethodCallExpr *main_class() const noexcept;

    void accept(Visitor &visitor) const noexcept override;

    ~Program() override;

private:
    std::unique_ptr<ProgramDeclaration> class_declarations_;
    std::unique_ptr<MethodCallExpr> main_class_;
};

class Visitor {
public:
    virtual ~Visitor() = 0;

    virtual void operator()(const NodeBase &node_base) = 0;

    virtual void operator()(const Expr &expr) = 0;

    virtual void operator()(const PrimaryExpr &primary_expr) = 0;

    virtual void operator()(const BooleanLiteralExpr &boolean_literal_expr) = 0;

    virtual void operator()(const IntegerLiteralExpr &integer_literal_expr) = 0;

    virtual void operator()(const RealLiteralExpr &real_literal_expr) = 0;

    virtual void operator()(const StringLiteralExpr &string_literal_expr) = 0;

    virtual void operator()(const ThisExpr &this_expr) = 0;

    virtual void operator()(const MemberAccessExpr &member_access_expr) = 0;

    virtual void operator()(const FieldAccessExpr &field_access_expr) = 0;

    virtual void operator()(const MethodCallExpr &method_call_expr) = 0;

    virtual void operator()(const MemberAccess &member_access) = 0;

    virtual void operator()(const BodyExpr &body_expr) = 0;

    virtual void operator()(const Body &body) = 0;

    virtual void operator()(const Stmt &stmt) = 0;

    virtual void operator()(const ReturnStmt &return_stmt) = 0;

    virtual void operator()(const AssignmentStmt &assignment_stmt) = 0;

    virtual void operator()(const IfStmt &if_stmt) = 0;

    virtual void operator()(const WhileStmt &while_stmt) = 0;

    virtual void operator()(const MemberDeclarationExpr &member_declaration_expr) = 0;

    virtual void operator()(const MemberDeclaration &member_declaration) = 0;

    virtual void operator()(const ParameterDeclaration &parameter_declaration) = 0;

    virtual void operator()(const VariableDeclaration &variable_declaration) = 0;

    virtual void operator()(const ConstructorDeclaration &constructor_declaration) = 0;

    virtual void operator()(const ConstructorDefinition &constructor_definition) = 0;

    virtual void operator()(const MethodDeclaration &method_declaration) = 0;

    virtual void operator()(const MethodDefinition &method_definition) = 0;

    virtual void operator()(const ProgramDeclarationExpr &program_declaration_expr) = 0;

    virtual void operator()(const ProgramDeclaration &program_declaration) = 0;

    virtual void operator()(const ClassDeclaration &class_declaration) = 0;

    virtual void operator()(const ClassDefinition &class_definition) = 0;

    virtual void operator()(const Program &program) = 0;
};

#endif
