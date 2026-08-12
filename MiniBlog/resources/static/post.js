async function loadPost()
{
    // URL looks like /posts/my-first-post — grab the last path segment.
    const segments = window.location.pathname.split("/").filter(Boolean);
    const id = segments[segments.length - 1];
    const container = document.getElementById("post");

    if(!id) {
        container.innerHTML = `<p class="not-found">Entry not found.</p>`;
        return;
    }

    const response = await fetch("/api/posts/" + id);

    if(!response.ok) {
        container.innerHTML = `<p class="not-found">Entry not found.</p>`;
        return;
    }

    const post = await response.json();
    const date = new Date(post.createdAt * 1000).toLocaleDateString(undefined, {
        year: "numeric",
        month: "short",
        day: "numeric"
    });

    container.innerHTML =
    `
    <div class="entry-meta">
        <span class="log-date">${date}</span>
    </div>

    <h1 class="entry-title">${post.title}</h1>

    <div class="entry-content">${post.content.replace(/\n/g, "<br>")}</div>
    `;

    document.title = post.title;
}

loadPost();