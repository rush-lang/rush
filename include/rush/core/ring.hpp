#pragma once

#ifndef RUSH_CORE_RING_HPP
#define RUSH_CORE_RING_HPP

#include <array>

namespace rush {


	template <typename T, std::size_t N>
	class ring {
		template <typename RanIter>
		class ring_iterator;

		template <typename RanIter>
		class ring_iterator {
			using iterator_type = RanIter;
			using ring_type = std::conditional_t<
				std::is_const_v<std::remove_reference_t<
					typename std::iterator_traits<RanIter>::reference>>,
					ring const, ring>;

		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = typename std::iterator_traits<iterator_type>::difference_type;
			using value_type = typename std::iterator_traits<iterator_type>::value_type;
			using reference = typename std::iterator_traits<iterator_type>::reference;
			using pointer = typename std::iterator_traits<iterator_type>::pointer;

			ring_iterator()
				: _owner(nullptr)
				, _iter()
				, _switch(false) {}

			ring_iterator(ring_type& owner, RanIter iter, bool end)
				: _owner(&owner)
				, _iter(iter)
				, _switch(end) {}

			reference operator *() const {
				return *_iter;
			}

			pointer operator ->() const {
				return std::addressof(this->operator*());
			}

			ring_iterator& operator ++() noexcept {
				advance();
				return *this;
			}

			ring_iterator operator ++(int) noexcept {
				auto temp = *this;
				advance();
				return temp;
			}

			ring_iterator& operator --() noexcept {
				retreat();
				return *this;
			}

			ring_iterator operator --(int) noexcept {
				auto temp = *this;
				retreat();
				return temp;
			}

			friend bool operator == (ring_iterator const& lhs, ring_iterator const& rhs) {
				return
					lhs._owner == rhs._owner &&
					lhs._switch == rhs._switch &&
					lhs._iter == rhs._iter;
			}

			friend bool operator != (ring_iterator const& lhs, ring_iterator const& rhs) {
				return !(lhs == rhs);
			}

		private:
			ring_type* _owner;
			RanIter _iter;
			bool _switch;

			void advance() {
				++_iter;
				if (_iter == _owner->_buf.end()) {
					_iter = _owner->_buf.begin();
					_switch = !_switch;
				}
			}

			void retreat() {
				if (_iter == _owner->_buf.begin()) {
					_iter = _owner->_buf.end();
					_switch = !_switch;
				}
				--_iter;
			}
		};

	public:

		using container_type = std::array<T, N>;
		using difference_type = typename container_type::difference_type;
		using value_type = typename container_type::value_type;
		using size_type = typename container_type::size_type;
		using reference = typename container_type::reference;
		using pointer = typename container_type::pointer;
		using const_reference = typename container_type::const_reference;
		using const_pointer = typename container_type::const_pointer;

		using iterator = ring_iterator<typename container_type::iterator>;
		using const_iterator = ring_iterator<typename container_type::const_iterator>;

		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	public:
		ring()
			: _buf {}
			, _head(_buf.begin())
			, _tail(_buf.begin())
			, _size(0) {}

		template <typename InIter>
		ring(InIter first, InIter last)
			: _buf {}
			, _head(_buf.begin())
			, _tail(_buf.begin())
			, _size(0) {
			std::copy(first, last, std::back_inserter(*this));
		}

		ring(std::initializer_list<T> init)
			: ring(init.begin(), init.end()) {}

		bool empty() const noexcept {
			return _size == 0;
		}

		size_type size() const noexcept {
			return _size;
		}

		size_type capacity() const noexcept {
			return _buf.size();
		}

		template <typename... Args>
		void emplace_back(Args&&... args) {

		}

		template <typename... Args>
		void emplace_front(Args&&... args) {

		}

		void push_back(T&& val) {
			_check_buffer_empty();
			*_tail = std::move(val);
			_advance_tail(!empty());
			_increment_size();
		}

		void push_back(T const& val) {
			_check_buffer_empty();
			*_tail = val;
			_advance_tail(!empty());
			_increment_size();
		}

		void push_front(T&& val) {
			_check_buffer_empty();
			_retract_head(!empty());
			*_head = std::move(val);
			_increment_size();
		}

		void push_front(T const& val) {
			_check_buffer_empty();
			_retract_head(!empty());
			*_head = val;
			_increment_size();
		}

		void pop_back() {
			_check_buffer_empty();
			if (!empty()) {
				_retract_tail(false);
				_decrement_size();
			}
		}

		void pop_front() {
			_check_buffer_empty();
			if (!empty()) {
				_advance_head(false);
				_decrement_size();
			}
		}

		reference front() noexcept {
			return *_head;
		}

		reference back() noexcept {
			return _tail == _buf.begin()
				? *(_buf.end() - 1)
				: *(_tail - 1);
		}

		const_reference front() const noexcept {
			return *_head;
		}

		const_reference back() const noexcept {
			return _tail == _buf.begin()
				? *(_buf.end() - 1)
				: *(_tail - 1);
		}

		// implement swap.

		iterator begin() noexcept { return _make_begin(empty()); }
		iterator end() noexcept { return _make_end(true); }
		const_iterator begin() const noexcept { return _make_begin(empty()); }
		const_iterator end() const noexcept { return _make_end(true); }
		const_iterator cbegin() const noexcept { return _make_begin(empty()); }
		const_iterator cend() const noexcept { return _make_end(true); }

		reverse_iterator rbegin() noexcept { return reverse_iterator { _make_end(empty()) }; }
		reverse_iterator rend() noexcept { return reverse_iterator { _make_begin(true) }; }
		const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator { _make_end(empty()) }; }
		const_reverse_iterator rend() const noexcept { return const_reverse_iterator { _make_begin(true) }; }
		const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator { _make_end(empty()) }; }
		const_reverse_iterator crend() const noexcept { return const_reverse_iterator { _make_begin(true) }; }


	private:
		container_type _buf;
		typename container_type::iterator
			_head,
			_tail;
		size_type _size;

		void _check_buffer_empty() {
			if (_buf.empty())
				throw std::out_of_range("empty ring buffer.");
		}

		void _increment_size() {
			if (_size != capacity()) ++_size;
		}

		void _decrement_size() {
			if (_size != 0) --_size;
		}

		void _advance_tail(bool b) {
			if (b && _tail == _head)
			{ _advance_head(false); }
			++_tail;
			if (_tail == _buf.end())
			{ _tail = _buf.begin(); }
		}

		void _advance_head(bool b) {
			if (b && _head == _tail)
			{ _advance_tail(false); }
			++_head;
			if (_head == _buf.end())
			{ _head = _buf.begin(); }
		}

		void _retract_head(bool b) {
			if (b && _head == _tail)
			{ _retract_tail(false); }
			if (_head == _buf.begin())
			{ _head = _buf.end(); }
			--_head;
		}

		void _retract_tail(bool b) {
			if (b && _tail == _head)
			{ _retract_head(false); }
			if (_tail == _buf.begin())
			{ _tail = _buf.end(); }
			--_tail;
		}

		iterator _make_begin(bool end) {
			return { *this, _head, end };
		}

		iterator _make_end(bool end) {
			return { *this, _tail, end };
		}

		const_iterator _make_begin(bool end) const {
			return { *this, _head, end };
		}

		const_iterator _make_end(bool end) const {
			return { *this, _tail, end };
		}
	};
}

#endif // RUSH_CORE_RING_HPP
