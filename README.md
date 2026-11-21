# MiniSpring

**MiniSpring** is a lightweight, Spring-inspired backend framework written from scratch in **C++17**.

The project explores how the core concepts of modern backend frameworks can be implemented in C++, providing an integrated set of components for building backend applications.

## Features

- **HTTP Server** — Custom HTTP server with configurable ports, worker threads, and keep-alive connections.
- **Routing** — HTTP route registration, path parameters, and controller-based request handling.
- **Reflection** — Custom runtime type metadata and entity registration system.
- **ORM** — Object-relational mapping with automatic SQL generation and entity management.
- **Database** — SQL database integration, currently supporting SQLite.
- **Migrations** — Database schema migration support.
- **JSON** — JSON parsing, serialization, and object mapping.
- **Security** — Authentication and authorization infrastructure, including route-level access protection.

## Example

### Application Configuration

MiniSpring uses a modular application structure. Modules can be registered and configured independently before the application is built and started.

```cpp
#include <MiniSpring.h>

int main() {
    MiniSpringApp app;

    // Configure database
    auto& database = app.modules().add<DatabaseModule>();
    database.configuration()
        .file("blog.db")
        .migrations("resources/migrations");

    // Configure HTTP server
    auto& http = app.modules().add<HttpModule>();
    http.configuration()
        .port(8080)
        .threads(4)
        .keepAlive(true);

    // Configure security
    auto& security = app.modules().add<SecurityModule>();
    security.configuration()
        .cookieName("SESSIONID")
        .frameOptions(FrameOptions::DENY);

    // Register entities
    app.services().get<EntityRegistry>()
        .registerEntity<Post>()
        .registerEntity<User>();

    // Initialize and build application
    app.configure();
    app.build();

    // Run database migrations
    database.migrate();

    // Register controllers
    http.controllers().registerController<PostController>(app);
    http.controllers().registerController<AuthController>(app);

    // Start server
    app.listen();
}
```

### Controller and Routing

Controllers define HTTP endpoints and handle incoming requests. Routes can use path parameters, typed JSON deserialization, and security rules.

```cpp
class PostController : public Controller {
public:
    PostController(MiniSpringApp& app)
        : m_service(app.services().get<PostService>()),
          m_mapper(app.services().get<JsonMapper>()),
          m_security(app.modules().get<SecurityModule>()) {}

    void registerRoutes(Router& router) override {
        // Retrieve all posts
        router.addRoute("GET", "/api/posts", [this](HttpRequest& req) {
            HttpResponse res;

            auto posts = m_service.getAllPosts();
            res.json(m_mapper.toJson(posts));

            return Result<HttpResponse, HttpError>::success(res);
        });

        // Retrieve a post by ID
        router.addRoute("GET", "/api/posts/{id}", [this](HttpRequest& req) {
            HttpResponse res;

            std::string id = req.pathParameter("id");
            auto post = m_service.getPost(id);

            if (!post)
                return Result<HttpResponse, HttpError>::error(
                    HttpError(404, "Post not found")
                );

            res.json(m_mapper.toJson(post.value()));

            return Result<HttpResponse, HttpError>::success(res);
        });

        // Create a post (ADMIN access required)
        router.addRoute("POST", "/api/posts", [this](HttpRequest& req) {
            HttpResponse res;

            PostCreateRequest request = req.json<PostCreateRequest>();
            Post post = m_service.create(request);

            res.json(m_mapper.toJson(post));

            return Result<HttpResponse, HttpError>::success(res);
        }, m_security.protect({ "ADMIN" }));
    }

private:
    PostService& m_service;
    JsonMapper& m_mapper;
    SecurityModule& m_security;
};
```

The examples above are simplified excerpts from a sample application built with MiniSpring.

## Project

MiniSpring was developed as a personal engineering project to explore backend framework architecture and implement concepts commonly found in frameworks such as Spring Boot using C++.

The framework is currently used as backend infrastructure in other software projects.

**Language:** C++17  
**Database:** SQLite (currently supported)
