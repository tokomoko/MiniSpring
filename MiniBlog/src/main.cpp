#include<MiniSpring.h>

#include"models/Post.h"
#include"models/PostCreateRequest.h"
#include"models/PostUpdateRequest.h"
#include"models/User.h"
#include"models/LoginRequest.h"

#include"controllers/PostController.h"
#include"controllers/AuthController.h"

#include"security/AppUserDetailsService.h"
#include"security/AdminSeeder.h"

#include<iostream>

int main() {
	try {
		MiniSpringApp app;

		auto& database = app.modules().add<DatabaseModule>();
		database.configuration()
			.file("blog.db")
			.migrations("resources/migrations");

		auto& http = app.modules().add<HttpModule>();
		http.configuration()
			.port(8080)
			.threads(1)
			.keepAlive(true);

		http.staticFiles().directory("resources/static");

		auto& security = app.modules().add<SecurityModule>();
		security.configuration()
			.cookieName("SESSIONID")
			.frameOptions(FrameOptions::DENY)
			.referrerPolicy(ReferrerPolicy::STRICT_ORIGIN_WHEN_CROSS_ORIGIN);

		app.services().get<EntityRegistry>()
			.registerEntity<Post>()
			.registerEntity<PostCreateRequest>()
			.registerEntity<PostUpdateRequest>()
			.registerEntity<User>()
			.registerEntity<LoginRequest>();

		app.services().add<MarkdownRenderer>(std::make_unique<MarkdownRenderer>());
		app.services().add<IPasswordEncoder>(std::make_unique<PBKDF2PasswordEncoder>());
		app.services().add<IUserDetailsService>(std::make_unique<AppUserDetailsService>(app.services()));

		app.configure();
		app.build();

		database.migrate();
		AdminSeeder::seed(app.services());

		auto repository = std::make_unique<PostRepository>(app.services().get<EntityManager>());
		auto service = std::make_unique<PostService>(*repository);

		app.services().add<PostRepository>(std::move(repository));
		app.services().add<PostService>(std::move(service));

		http.controllers().registerController<PostController>(app);
		http.controllers().registerController<AuthController>(app);

		app.listen();
	}
	catch (const DatabaseException& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (const HttpException& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (...) {}
}