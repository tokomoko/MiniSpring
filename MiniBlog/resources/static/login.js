const form = document.getElementById('loginForm');
const errorEl = document.getElementById('error');
const submitBtn = document.getElementById('submitBtn');

form.addEventListener('submit', async (e) => {
    e.preventDefault();
    errorEl.textContent = '';
    submitBtn.disabled = true;

    try {
        const res = await fetch('/api/login', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            credentials: 'same-origin',
            body: JSON.stringify({
                username: document.getElementById('username').value,
                password: document.getElementById('password').value
            })
        });

        if (!res.ok) {
            const body = await res.json().catch(() => ({}));
            errorEl.textContent = body.message || 'Invalid username or password';
            submitBtn.disabled = false;
            return;
        }

        window.location.href = '/';
    } catch (err) {
        errorEl.textContent = 'Could not reach the server. Try again.';
        submitBtn.disabled = false;
    }
});