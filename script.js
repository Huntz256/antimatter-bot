function isAlive(cell)
{
    return cell.alive ? '1' : '0';
}
function rowToString(row)
{
    return row.map(isAlive).join('');
}
function gridToString()
{
    return app.game.grid.cells.map(rowToString).join('');
}
function place(i, j)
{
   app.gameClient.placeLiveCells([app.renderer.grid.getCell(i % 100, j % 50)]);
}
