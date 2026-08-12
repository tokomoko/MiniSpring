let allPosts = [];
let editingPostId = null;
let isAuthenticated = false;

async function checkAuth()
{
    const authSlot = document.getElementById("auth-slot");
    const newEntryBtn = document.getElementById("new-entry-toggle");

    try {
        const response = await fetch("/api/me", { credentials: "same-origin" });

        if (response.ok) {
            const me = await response.json();
            isAuthenticated = true;

            authSlot.innerHTML =
            `
            <span class="auth-user">Signed in as <span class="username">${me.username}</span></span>
            <button id="signout-btn" class="signout-btn">Sign out</button>
            `;

            newEntryBtn.classList.remove("hidden");
            return;
        }
    } catch (err) {
        // network error -- treat as logged out below
    }

    isAuthenticated = false;
    authSlot.innerHTML = `<a href="/login" class="signin-link">Sign in</a>`;
    newEntryBtn.classList.add("hidden");
}

async function signOut()
{
    await fetch("/api/logout", { method: "POST", credentials: "same-origin" });
    window.location.href = "/login";
}

async function loadPosts()
{
    const response = await fetch("/api/posts");
    const text = await response.text();
    allPosts = JSON.parse(text);

    renderIndex();
    renderLog();
}

function renderIndex()
{
    const sorted = [...allPosts].sort((a, b) => a.title.localeCompare(b.title));
    const list = document.getElementById("index-list");
    list.innerHTML = "";

    sorted.forEach((post, i) => {
        const li = document.createElement("li");
        li.className = "index-item";
        li.innerHTML =
        `
        <span class="tag">P${pad(i + 1)}</span>
        <a href="/posts/${post.id}">${post.title}</a>
        `;
        list.appendChild(li);
    });
}

function renderLog()
{
    const sorted = [...allPosts].sort((a, b) => b.createdAt - a.createdAt);

    const chronological = [...allPosts].sort((a, b) => a.createdAt - b.createdAt);
    const numberOf = {};
    chronological.forEach((post, i) => { numberOf[post.id] = i + 1; });

    const list = document.getElementById("log-list");
    list.innerHTML = "";

    sorted.forEach(post => {
        const li = document.createElement("li");
        li.className = "log-card";
        li.dataset.postId = post.id; // used by the delegated click handler below

        const adminActions = isAuthenticated
            ? `<button class="edit" data-action="edit">Edit</button>
               <button class="delete" data-action="delete">Delete</button>`
            : "";

        li.innerHTML =
        `
        <div class="log-meta">
            <span class="tag">#${pad(numberOf[post.id])}</span>
            <span class="log-date">${formatDate(post.createdAt)}</span>
        </div>

        <a class="log-title" href="/posts/${post.id}">${post.title}</a>

        <p class="log-excerpt">${excerpt(post.content, 220)}</p>

        <div class="log-actions">
            <a class="read-more" href="/posts/${post.id}">Read entry &rarr;</a>
            ${adminActions}
        </div>
        `;
        list.appendChild(li);
    });
}

function pad(n)
{
    return n.toString().padStart(2, "0");
}

function formatDate(unixSeconds)
{
    return new Date(unixSeconds * 1000).toLocaleDateString(undefined, {
        year: "numeric",
        month: "short",
        day: "numeric"
    });
}

function excerpt(content, max)
{
    if(content.length <= max) return content;
    return content.slice(0, max) + "...";
}

function toggleComposer()
{
    document.getElementById("composer").classList.toggle("hidden");
}

async function deletePost(id)
{
    const response = await fetch("/api/posts/" + id, {
        method: "DELETE",
        credentials: "same-origin"
    });

    if (!response.ok) {
        alert("Could not delete entry (are you still signed in?)");
        return;
    }

    loadPosts();
}

async function submitPost()
{
    const titleInput = document.getElementById("title");
    const contentInput = document.getElementById("content");
    const title = titleInput.value.trim();
    const content = contentInput.value.trim();

    if(title === "")
        return;

    let url;
    let method;

    if(editingPostId)
    {
        url = "/api/posts/" + editingPostId;
        method = "PUT";
    }
    else
    {
        url = "/api/posts";
        method = "POST";
    }

    const response = await fetch(url, {
        method: method,
        credentials: "same-origin",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ title: title, content: content })
    });

    if (!response.ok) {
        alert("Could not save entry (are you still signed in?)");
        return;
    }

    cancelEdit();
    loadPosts();
}

function editPost(id)
{
    const post = allPosts.find(p => p.id === id);

    if(!post) return;

    editingPostId = id;
    document.getElementById("title").value = post.title;
    document.getElementById("content").value = post.content;
    document.getElementById("composer").classList.remove("hidden");
    document.getElementById("publish-button").innerText = "Save changes";
    document.getElementById("cancel-button").classList.remove("hidden");
    window.scrollTo({top:0, behavior:"smooth"});
}

function cancelEdit()
{
    editingPostId = null;
    document.getElementById("title").value = "";
    document.getElementById("content").value = "";
    document.getElementById("composer").classList.add("hidden");
    document.getElementById("publish-button").innerText = "Publish entry";
    document.getElementById("cancel-button").classList.add("hidden");
}

// --- Event bindings (replaces inline onclick="..." attributes, which CSP's
// script-src-attr / default-src blocks) ---

document.getElementById("new-entry-toggle").addEventListener("click", toggleComposer);
document.getElementById("publish-button").addEventListener("click", submitPost);
document.getElementById("cancel-button").addEventListener("click", cancelEdit);

// Delegated handlers: log-list and auth-slot get their contents replaced
// wholesale on every render, so binding directly to individual buttons
// would mean re-binding (and leaking) listeners every reload. Binding once
// to the stable parent and checking event.target avoids that.

document.getElementById("log-list").addEventListener("click", (e) => {
    const button = e.target.closest("button[data-action]");
    if (!button) return;

    const postId = button.closest(".log-card").dataset.postId;
    if (button.dataset.action === "edit") editPost(postId);
    else if (button.dataset.action === "delete") deletePost(postId);
});

document.getElementById("auth-slot").addEventListener("click", (e) => {
    if (e.target.id === "signout-btn") signOut();
});

checkAuth();
loadPosts();