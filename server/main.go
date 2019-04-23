package main

import (
	"encoding/json"
	"fmt"
	"github.com/jmoiron/sqlx"
	_ "github.com/lib/pq"
	"net/http"
	"os"
	"strconv"
	"strings"
	"time"
)

func main() {
	db, err := sqlx.Connect("postgres", "")
	assert(err)

	port := os.Getenv("SERVER_PORT")
	if len(port) == 0 {
		port = "8080"
	}

	repo := Repository{db}
	handler := Handler{&repo}
	http.Handle("/quotes", &handler)
	assert(http.ListenAndServe(fmt.Sprintf(":%s", port), nil))
}

type Handler struct {
	repo *Repository
}

func (h *Handler) ServeHTTP(resp http.ResponseWriter, req *http.Request) {
	symbols := strings.Split(strings.ToUpper(req.URL.Query().Get("symbols")), ",")
	if len(symbols) == 0 {
		// todo
		return
	}

	from, err := h.toTime(req.URL.Query().Get("from"))
	if err != nil {
		// todo
		return
	}

	to, err := h.toTime(req.URL.Query().Get("to"))
	if err != nil {
		// todo
		return
	}

	companies, err := h.repo.companies(symbols)
	if err != nil {
		// todo
		return
	}

	quotes, err := h.repo.quotes(symbols, from, to)
	if err != nil {
		// todo
		return
	}

	for i := range companies {
		companies[i].Quotes = quotes[companies[i].Symbol]
	}
	body, err := json.Marshal(companies)
	if err != nil {
		// todo
		return
	}

	_, err = resp.Write(body)
	if err != nil {
		// todo
	}
}

func (h *Handler) toTime(s string) (time.Time, error) {
	var res time.Time
	if len(s) == 0 {
		return res, nil
	}

	val, err := strconv.Atoi(s)
	if err != nil {
		return res, nil
	}

	return time.Unix(int64(val), 0), nil
}

type Repository struct {
	db *sqlx.DB
}

func (c *Repository) companies(symbols []string) ([]Company, error) {
	query, args, err := sqlx.In("SELECT * FROM companies WHERE symbol IN (?)", symbols)
	if err != nil {
		return nil, err
	}

	var data []Company
	err = c.db.Select(&data, c.db.Rebind(query), args...)
	if err != nil {
		return nil, err
	}

	return data, nil
}

func (c *Repository) quotes(
	symbols []string,
	from time.Time,
	to time.Time,
) (map[string][]Quote, error) {
	query, args, err := sqlx.In("SELECT * FROM quotes WHERE symbol IN (?)", symbols)
	if err != nil {
		return nil, err
	}

	if !from.IsZero() {
		query += " AND ? <= updated"
		args = append(args, from)
	}
	if !to.IsZero() {
		query += " AND updated <= ?"
		args = append(args, to)
	}
	rows, err := c.db.Query(c.db.Rebind(query), args...)
	if err != nil {
		return nil, err
	}

	res := make(map[string][]Quote)
	for rows.Next() {
		var symbol string
		var quote Quote
		err = rows.Scan(&symbol, &quote.Price, &quote.Updated)
		if err != nil {
			return nil, err
		}

		res[symbol] = append(res[symbol], quote)
	}
	return res, nil
}

type Company struct {
	Symbol string  `json:"symbol" db:"symbol"`
	Name   string  `json:"name" db:"name"`
	Logo   string  `json:"logo" db:"logo"`
	Quotes []Quote `json:"quotes"`
}

type Quote struct {
	Price   float64   `json:"price" db:"price"`
	Updated time.Time `json:"updated" db:"updated"`
}

func assert(err error) {
	if err != nil {
		panic(err)
	}
}
