(async () => {
  const input = document.getElementById('word');
  const meaning = document.getElementById('meaning');
  const status = document.getElementById('status');
  const suggestions = document.getElementById('suggestions');
  try {
    const module = await createLexiTrieModule();
    const insert = module.cwrap('lex_insert', 'number', ['string','string']);
    const lookup = module.cwrap('lex_lookup', 'string', ['string']);
    const suggest = module.cwrap('lex_suggest', 'string', ['string','number']);
    const response = await fetch('./dictionary.txt');
    if (!response.ok) throw new Error('Không tải được dictionary.txt');
    const dictionary = await response.text();
    let count = 0;
    for (const line of dictionary.split(/\r?\n/)) {
      const split = line.indexOf('|');
      if (split > 0 && insert(line.slice(0,split),line.slice(split+1))) count++;
    }
    status.textContent = `Đã nạp ${count.toLocaleString('vi-VN')} từ vào Trie C++ WebAssembly.`;
    meaning.textContent = 'Nhập từ để xem nghĩa tiếng Việt.';
    input.disabled = false;
    input.placeholder = 'Ví dụ: hello, apple, computer';
    input.addEventListener('input', () => {
      const word = input.value.trim().toLowerCase();
      suggestions.replaceChildren();
      if (!word) { meaning.textContent = 'Nhập từ để xem nghĩa tiếng Việt.'; return; }
      const found = lookup(word);
      meaning.textContent = found || 'Không tìm thấy từ trong từ điển.';
      for (const candidate of suggest(word, 10).split('\n').filter(Boolean)) {
        const button = document.createElement('button');
        button.textContent = candidate;
        button.onclick = () => { input.value = candidate; input.dispatchEvent(new Event('input')); };
        suggestions.appendChild(button);
      }
    });
  } catch (error) {
    meaning.textContent = 'Không khởi tạo được từ điển: ' + error.message;
    status.textContent = 'Lỗi tải WebAssembly hoặc dữ liệu.';
  }
})();
