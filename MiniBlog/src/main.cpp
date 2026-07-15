#include<MiniSpring.h>

#include"models/Post.h"
#include"models/PostCreateRequest.h"
#include"models/PostUpdateRequest.h"

#include"repositories/PostRepository.h"

#include"services/PostService.h"

#include"controllers/PostController.h"

int main() {
	try {
		MiniSpringApp app;

		auto& database = app.modules().add<DatabaseModule>();
		database.configuration()
			.file(executableDirectory()/"blog.db")
			.migrations(executableDirectory()/"resources/migrations");

		auto& http = app.modules().add<HttpModule>();
		http.configuration()
			.port(8080)
			.threads(4);

		http.staticFiles().directory(executableDirectory()/"resources/static");

		app.services().get<EntityRegistry>()
			.registerEntity<Post>()
			.registerEntity<PostCreateRequest>()
			.registerEntity<PostUpdateRequest>();


		app.configure();
		app.build();

		app.services().add<MarkdownRenderer>(std::make_unique<MarkdownRenderer>());

		auto& entityManager = app.services().get<EntityManager>();
		app.services().add<PostRepository>(std::make_unique<PostRepository>(entityManager));

		auto& repository = app.services().get<PostRepository>();
		app.services().add<PostService>(std::make_unique<PostService>(repository));

		database.migrate();
		http.controllers().registerController<PostController>(app);

		app.listen();
	}
	catch (const DatabaseException& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (const HttpException& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}